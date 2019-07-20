#pragma once

#include "Common.hpp"

#include "Background.hpp"
#include "Camera.hpp"
#include "Framebuffer.hpp"
#include "Material.hpp"
#include "Math.hpp"
#include "Random.hpp"
#include "Ray.hpp"
#include "Sampling.hpp"
#include "Scene.hpp"
#include "Vec3.hpp"
#include "Viewport.hpp"

#include <atomic>
#include <future>
#include <limits>

class Raytracer
{
private:
	bool visualiseDepth = false;
	bool visualiseBounces = false;
	uint maxBounces = 50;
	uint samplesPerPixel = 100;

	Vec3 gammaCorrect(const Vec3 &colour) const;

public:
	Raytracer() {}

	Vec3 getColour(const Ray &r, const Scene &scene, uint bounces = 0) const;
	void renderPixels(const Camera &camera, const Scene &scene, const Viewport &vp, std::atomic<uint> &counter, Framebuffer &framebuffer) const;
	void renderBatchedPixels(const Camera &camera, const Scene &scene, const Viewport &vp, std::atomic<uint> &counter, Framebuffer &framebuffer, uint batchSize) const;
	void render(const Scene &scene, const Camera &camera, Framebuffer &framebuffer) const;
	void setVisualiseDepth(bool value) { visualiseDepth = value; }
	void setVisualiseBounces(bool value) { visualiseBounces = value; }
	void setMaxBounces(uint n) { maxBounces = n; }
	void setSamplesPerPixel(uint n) { samplesPerPixel = n; }
};

Vec3 Raytracer::gammaCorrect(const Vec3 &colour) const
{
	// Gamma 2 correction
	return sqrt(colour);
}

Vec3 Raytracer::getColour(const Ray &r, const Scene &scene, uint bounces) const
{
	HitRecord rec;
	bool hit = scene.hit(r, 0.001, maxReal(), rec);

	// Visualisations hijack the recursion train
	if (visualiseDepth)
	{
		return Vec3(1, 1, 1) / (1.0 + rec.t);
	}
	else if (visualiseBounces)
	{
		if (hit)
		{
			Ray scattered;
			Vec3 attenuation;
			if (bounces < maxBounces && rec.material && rec.material->scatter(r, rec, attenuation, scattered))
				return getColour(scattered, scene, bounces + 1);
		}
		return Vec3(1, 1, 1) * (Real(bounces) / Real(maxBounces));
	}

	// Normal rendering
	if (hit)
	{
		Ray scattered;
		Vec3 attenuation;
		if (bounces < maxBounces && rec.material && rec.material->scatter(r, rec, attenuation, scattered))
			return getColour(scattered, scene, bounces + 1) * attenuation;
		else
			return Vec3();
	}

	return scene.background().sample(r.direction());
}

void Raytracer::renderBatchedPixels(const Camera &camera, const Scene &scene, const Viewport &vp, std::atomic<uint> &counter, Framebuffer &framebuffer, uint batchSize) const
{
	uint pixelAmount = vp.width() * vp.height();
	uint pixelBatchAmount = max(pixelAmount / batchSize, 1);
	uint pixelBatchIndex = counter++;
	while (pixelBatchIndex < pixelBatchAmount)
	{
		uint indexStop = min(int((pixelBatchIndex + 1) * batchSize), int(pixelAmount));
		for (uint pixelIndex = pixelBatchIndex * batchSize; pixelIndex < indexStop; pixelIndex++)
		{
			uint row = pixelIndex / vp.width();
			uint col = pixelIndex % vp.width();
			Vec3 colour;
			for (uint i = 0; i < samplesPerPixel; i++)
			{
				Real u = Real(col + uniformRand()) / vp.width();
				Real v = Real(row + uniformRand()) / vp.height();
				Ray r = camera.getRay(u, v);
				colour += getColour(r, scene);
			}
			colour /= samplesPerPixel;
			colour = 255.99 * gammaCorrect(colour);

			framebuffer.store(int(col), int(row), colour);
		}

		pixelBatchIndex = counter++;
	}
}

void Raytracer::renderPixels(const Camera &camera, const Scene &scene, const Viewport &vp, std::atomic<uint> &counter, Framebuffer &framebuffer) const
{
	uint pixelAmount = vp.width() * vp.height();
	uint pixelIndex = counter++;
	while (pixelIndex < pixelAmount)
	{
		uint row = pixelIndex / vp.width();
		uint col = pixelIndex % vp.width();
		Vec3 colour;
		for (uint i = 0; i < samplesPerPixel; i++)
		{
			Real u = Real(col + uniformRand()) / vp.width();
			Real v = Real(row + uniformRand()) / vp.height();
			Ray r = camera.getRay(u, v);
			colour += getColour(r, scene);
		}
		colour /= samplesPerPixel;
		colour = 255.99 * gammaCorrect(colour);

		framebuffer.store(int(col), int(row), colour);

		pixelIndex = counter++;
	}
}

void Raytracer::render(const Scene &scene, const Camera &camera, Framebuffer &framebuffer) const
{
    Camera myCamera = camera;
    if (visualiseDepth || visualiseBounces)
        myCamera.setDepthOfFieldEnabled(false);
    
	std::atomic<uint> renderPixelCounter {0};
	const Viewport &viewport = myCamera.getViewport();

	const uint nThreads = max(std::thread::hardware_concurrency(), 1) - 1;
	std::future<void> futures[nThreads];
	for (uint i = 0; i < nThreads; i++)
		futures[i] = std::async(std::launch::async, &Raytracer::renderPixels, this, std::ref(myCamera), std::ref(scene), std::ref(viewport), std::ref(renderPixelCounter), std::ref(framebuffer));

	renderPixels(myCamera, scene, viewport, renderPixelCounter, framebuffer);

	for (uint i = 0; i < nThreads; i++)
		futures[i].get();
}

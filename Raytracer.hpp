#pragma once

#include "Common.hpp"

#include "Background.hpp"
#include "Camera.hpp"
#include "Framebuffer.hpp"
#include "Material.hpp"
#include "Math.hpp"
#include "Ray.hpp"
#include "Scene.hpp"
#include "Vec3.hpp"
#include "Viewport.hpp"

#include <atomic>
#include <limits>
#include <thread>

class Raytracer
{
private:
	uint maxDepth = 50;
	uint samplesPerPixel = 100;

	Vec3 gammaCorrect(const Vec3 &colour) const;

public:
	Raytracer() {}

	Vec3 getColour(const Ray &r, const Scene &scene, uint depth = 0) const;
	void renderPixels(const Camera &camera, const Scene &scene, const Viewport &vp, std::atomic<uint> &counter, Framebuffer &framebuffer) const;
	void render(const Scene &scene, const Camera &camera, Framebuffer &framebuffer) const;
	void setMaxDepth(uint d) { maxDepth = d; }
	void setSamplesPerPixel(uint n) { samplesPerPixel = n; }
};

Vec3 Raytracer::gammaCorrect(const Vec3 &colour) const
{
	// Gamma 2 correction
	return sqrt(colour);
}

Vec3 Raytracer::getColour(const Ray &r, const Scene &scene, uint depth) const
{
	HitRecord rec;
	if (scene.hit(r, 0.001, std::numeric_limits<Real>::max(), rec))
	{
		Ray scattered;
		Vec3 attenuation;
		if (depth < maxDepth && rec.material && rec.material->scatter(r, rec, attenuation, scattered))
			return getColour(scattered, scene, depth + 1) * attenuation;
		else
			return Vec3();
	}

	return scene.background().sample(r.direction());
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
	std::atomic<uint> renderPixelCounter {0};
	const Viewport &viewport = camera.getViewport();

	const uint nThreads = max(std::thread::hardware_concurrency(), 1) - 1;
	std::thread threads[nThreads];
	for (uint i = 0; i < nThreads; i++)
		threads[i] = std::thread(&Raytracer::renderPixels, this, std::ref(camera), std::ref(scene), std::ref(viewport), std::ref(renderPixelCounter), std::ref(framebuffer));

	renderPixels(camera, scene, viewport, renderPixelCounter, framebuffer);

	for (uint i = 0; i < nThreads; i++)
		threads[i].join();
}

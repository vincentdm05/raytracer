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
	void renderTile(const Camera &camera, const Scene &scene, const Viewport &vp, uint tileX, uint tileY, uint tileSize, Framebuffer<Vec3> &framebuffer) const;
	void renderTiles(const Camera &camera, const Scene &scene, const Viewport &vp, std::atomic<uint> &counter, Framebuffer<Vec3> &framebuffer) const;
	void renderPixels(const Camera &camera, const Scene &scene, const Viewport &vp, std::atomic<uint> &counter, Framebuffer<Vec3> &framebuffer) const;
	void render(const Scene &scene, const Camera &camera, Framebuffer<Vec3> &framebuffer) const;
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

void Raytracer::renderTile(const Camera &camera, const Scene &scene, const Viewport &vp, uint tileX, uint tileY, uint tileSize, Framebuffer<Vec3> &framebuffer) const
{
	uint tileOffsetX = tileX * tileSize;
	uint tileOffsetY = tileY * tileSize;

	uint stopX = min(tileOffsetX + tileSize, vp.width());
	uint stopY = min(tileOffsetY + tileSize, vp.height());
	for (uint row = tileOffsetY; row < stopY; row++)
	{
		for (uint col = tileOffsetX; col < stopX; col++)
		{
			Real u = Real(col);
			Real v = Real(row);
			if (samplesPerPixel > 1)
			{
				u += uniformRand();
				v += uniformRand();
			}
			else
			{
				u += 0.5;
				v += 0.5;
			}
			u *= vp.widthInv();
			v *= vp.heightInv();
			Ray r = camera.getRay(u, v);
			Vec3 colour = getColour(r, scene);

			for (uint i = 1; i < samplesPerPixel; i++)
			{
				u = Real(col + uniformRand()) * vp.widthInv();
				v = Real(row + uniformRand()) * vp.heightInv();
				r = camera.getRay(u, v);
				colour += getColour(r, scene);
			}

			colour /= samplesPerPixel;
			colour = 255.99 * gammaCorrect(colour);

			framebuffer.store(int(col), int(row), colour);
		}
	}
}

void Raytracer::renderTiles(const Camera &camera, const Scene &scene, const Viewport &vp, std::atomic<uint> &counter, Framebuffer<Vec3> &framebuffer) const
{
	constexpr uint tileSize = 16;
	uint tilesX = (vp.width() + tileSize - 1) / tileSize;
	uint tilesY = (vp.height() + tileSize - 1) / tileSize;
	uint tileAmount = tilesX * tilesY;
	while (true)
	{
		uint tileIndex = counter++;
		if (tileIndex >= tileAmount)
			break;

		uint tileY = tileIndex / tilesX;
		uint tileX = tileIndex % tilesX;

		renderTile(camera, scene, vp, tileX, tileY, tileSize, framebuffer);
	}
}

void Raytracer::renderPixels(const Camera &camera, const Scene &scene, const Viewport &vp, std::atomic<uint> &counter, Framebuffer<Vec3> &framebuffer) const
{
	uint pixelAmount = vp.width() * vp.height();
	while (true)
	{
		uint pixelIndex = counter++;
		if (pixelIndex >= pixelAmount)
			break;

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
}

void Raytracer::render(const Scene &scene, const Camera &camera, Framebuffer<Vec3> &framebuffer) const
{
	Camera myCamera = camera;
	if (visualiseDepth || visualiseBounces)
		myCamera.setDepthOfFieldEnabled(false);

	std::atomic<uint> renderPixelCounter {0};
	const Viewport &viewport = myCamera.getViewport();

	const uint nThreads = max(std::thread::hardware_concurrency(), 1u) - 1;
	std::future<void> futures[nThreads];
	for (uint i = 0; i < nThreads; i++)
		futures[i] = std::async(std::launch::async, &Raytracer::renderPixels, this, std::ref(myCamera), std::ref(scene), std::ref(viewport), std::ref(renderPixelCounter), std::ref(framebuffer));

	renderPixels(myCamera, scene, viewport, renderPixelCounter, framebuffer);

	for (uint i = 0; i < nThreads; i++)
		futures[i].get();
}

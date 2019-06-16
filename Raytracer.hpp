#pragma once

#include "Common.hpp"

#include "Background.hpp"
#include "Camera.hpp"
#include "Material.hpp"
#include "Math.hpp"
#include "Random.hpp"
#include "Ray.hpp"
#include "Sampling.hpp"
#include "Scene.hpp"
#include "Vec3.hpp"
#include "Viewport.hpp"

#include <future>
#include <iostream>
#include <limits>
#include <random>

class Raytracer
{
private:
	bool outputEnabled = true;
	bool visualiseDepth = false;
	bool visualiseBounces = false;
	uint maxBounces = 50;
	uint samplesPerPixel = 100;

	Vec3 gammaCorrect(const Vec3 &colour) const;

public:
	Raytracer() {}

	Vec3 getColour(const Ray &r, const Scene &scene, uint bounces = 0) const;
	Vec3 samplePixel(const Camera &camera, const Scene &scene, int col, int row, const Viewport &vp, uint nSamples = 1) const;
	void printImage(const Scene &scene, const Camera &camera) const;
	void setOutputEnabled(bool value) { outputEnabled = value; }
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

Vec3 Raytracer::samplePixel(const Camera &camera, const Scene &scene, int col, int row, const Viewport &vp, uint nSamples) const
{
	Vec3 colour;
	for (uint i = 0; i < nSamples; i++)
	{
		Real u = Real(col + uniformRand()) / vp.width();
		Real v = Real(row + uniformRand()) / vp.height();
		Ray r = camera.getRay(u, v);
		colour += getColour(r, scene);
	}
	return colour / nSamples;
}

void Raytracer::printImage(const Scene &scene, const Camera &camera) const
{
	Camera myCamera = camera;
	if (visualiseDepth || visualiseBounces)
		myCamera.setDepthOfFieldEnabled(false);

	const int nThreads = 1;
	std::future<Vec3> futures[nThreads];
	int samplesPerThread = samplesPerPixel / nThreads;

	const Viewport &viewport = myCamera.getViewport();

	int nx = viewport.width();
	int ny = viewport.height();
	if (outputEnabled)
		std::cout << "P3\n" << nx << " " << ny << "\n255\n";
	for (int row = ny - 1; row >= 0; row--)
	{
		for (int col = 0; col < nx; col++)
		{
			Vec3 colour;
			if (nThreads > 1)
			{
				for (int s = 0; s < nThreads; s++)
					futures[s] = std::async(std::launch::async, &Raytracer::samplePixel, this, std::ref(myCamera), std::ref(scene), col, row, std::ref(viewport), samplesPerThread);

				for (int s = 0; s < nThreads; s++)
					colour += futures[s].get();
				colour /= nThreads;
			}
			else
			{
				colour = samplePixel(myCamera, scene, col, row, viewport, samplesPerPixel);
			}

			colour = 255.99 * gammaCorrect(colour);

			if (outputEnabled)
				std::cout << int(colour.r) << " " << int(colour.g) << " " << int(colour.b) << "\n";
		}
	}
}

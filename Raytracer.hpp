#pragma once

#include "Common.hpp"

#include "Camera.hpp"
#include "Material.hpp"
#include "Math.hpp"
#include "Ray.hpp"
#include "Scene.hpp"
#include "Vec3.hpp"
#include "Viewport.hpp"

#include <iostream>
#include <limits>
#include <random>

struct Background
{
	Vec3 bottom;
	Vec3 top;

	Background() {}
	Background(const Vec3 &_bottom, const Vec3 &_top) { bottom = _bottom; top = _top; }

	Background &operator=(const Background &other) { if (this != &other) { bottom = other.bottom; top = other.top; }; return *this; }
};

class Raytracer
{
private:
	Background background;

	Vec3 gammaCorrect(const Vec3 &colour) const;

public:
	Raytracer() {}
	Raytracer(const Background &_background) { background = _background; }

	Vec3 getColour(const Ray &r, const Scene &scene, int depth = 0) const;
	void printImage(const Scene &scene, const Camera &camera) const;
	void setBackground(const Background &_background) { background = _background; }
};

Vec3 Raytracer::gammaCorrect(const Vec3 &colour) const
{
	// Gamma 2 correction
	return sqrt(colour);
}

Vec3 Raytracer::getColour(const Ray &r, const Scene &scene, int depth) const
{
	HitRecord rec;
	if (scene.hit(r, 0.001, std::numeric_limits<Real>::max(), rec))
	{
		Ray scattered;
		Vec3 attenuation;
		if (depth < 50 && rec.material && rec.material->scatter(r, rec, attenuation, scattered))
			return getColour(scattered, scene, depth + 1) * attenuation;
		else
			return Vec3();
	}

	Vec3 d = normalize(r.direction());
	Real altitude = 0.5 * (d.y + 1.0);
	return lerp(background.bottom, background.top, altitude);
}

void Raytracer::printImage(const Scene &scene, const Camera &camera) const
{
	const Viewport &viewport = camera.getViewport();

	int nx = viewport.width();
	int ny = viewport.height();
	int ns = 100;
	std::cout << "P3\n" << nx << " " << ny << "\n255\n";
	for (int row = ny - 1; row >= 0; row--)
	{
		for (int col = 0; col < nx; col++)
		{
			Vec3 colour;
			for (int s = 0; s < ns; s++)
			{
				Real u = Real(col + drand48()) / nx;
				Real v = Real(row + drand48()) / ny;
				Ray r = camera.getRay(u, v);
				colour += getColour(r, scene);
			}
			colour = 255.99 * gammaCorrect(colour);
			std::cout << int(colour.r) << " " << int(colour.g) << " " << int(colour.b) << "\n";
		}
	}
}

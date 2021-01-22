#pragma once

#include "Common.hpp"

#include "Background.hpp"
#include "Camera.hpp"
#include "Framebuffer.hpp"
#include "Material.hpp"
#include "Math.hpp"
#include "PixelRenderer.hpp"
#include "Random.hpp"
#include "Ray.hpp"
#include "Scene.hpp"
#include "Vec3.hpp"
#include "Viewport.hpp"

class Raytrace : public PixelRenderer
{
protected:
	const Camera &camera;
	const Scene &scene;
	Framebuffer &framebuffer;
	uint maxBounces = 50;
	uint samplesPerPixel = 100;

	Vec3 gammaCorrect(const Vec3 &colour) const;
	Vec3 getColour(const Ray &r, const Scene &scene, uint bounces = 0) const;

public:
	Raytrace(const Scene &s, const Camera &cam, const Viewport &vp, Framebuffer &fb);

	virtual void renderPixel(uint col, uint row) const override;

	// Maximum number of times a ray is allowed to bounce off a surface
	void setMaxBounces(uint n) { maxBounces = n; }
	// Number of anti-aliasing multisample takes per pixel
	void setSamplesPerPixel(uint n) { samplesPerPixel = n; }
};

Vec3 Raytrace::gammaCorrect(const Vec3 &colour) const
{
	// Gamma 2 correction
	return sqrt(colour);
}

Vec3 Raytrace::getColour(const Ray &r, const Scene &scene, uint bounces) const
{
	HitRecord rec;
	bool hit = scene.hit(r, 0.001, maxReal(), rec);

	if (hit)
	{
		Ray scattered;
		Vec3 attenuation;
		Vec3 emission = rec.material ? rec.material->emitted(rec.point) : Vec3();
		if (bounces < maxBounces && rec.material && rec.material->scatter(r, rec, attenuation, scattered))
			return emission + getColour(scattered, scene, bounces + 1) * attenuation;
		else
			return emission;
	}

	return scene.background().sample(r.direction());
}

Raytrace::Raytrace(const Scene &s, const Camera &cam, const Viewport &vp, Framebuffer &fb)
: PixelRenderer(vp)
, camera(cam)
, scene(s)
, framebuffer(fb)
{}

void Raytrace::renderPixel(uint col, uint row) const
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
	u *= viewport.widthInv();
	v *= viewport.heightInv();
	Ray r = camera.getRay(u, v);
	Vec3 colour = getColour(r, scene);

	for (uint i = 1; i < samplesPerPixel; i++)
	{
		u = Real(col + uniformRand()) * viewport.widthInv();
		v = Real(row + uniformRand()) * viewport.heightInv();
		r = camera.getRay(u, v);
		colour += getColour(r, scene);
	}

	colour /= samplesPerPixel;
	colour = 255.99 * gammaCorrect(colour);

	Real colourArray[3];
	colourArray[0] = colour.r;
	colourArray[1] = colour.g;
	colourArray[2] = colour.b;
	framebuffer.store(int(col), int(row), (byte*)colourArray);
}

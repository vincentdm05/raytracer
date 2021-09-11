#pragma once

#include "Common.hpp"

#include "Background.hpp"
#include "Camera.hpp"
#include "Image.hpp"
#include "Material.hpp"
#include "Math.hpp"
#include "PixelRenderer.hpp"
#include "Postprocess.hpp"
#include "Random.hpp"
#include "Ray.hpp"
#include "Scene.hpp"
#include "Vec3.hpp"
#include "Viewport.hpp"

class Raymarch : public PixelRenderer
{
protected:
	const Camera &camera;
	const Scene &scene;
	Image &image;
	uint maxRayIterations = 300;
	uint maxBounces = 10;
	uint samplesPerPixel = 1;
	Real maxRayLength = 1000.0;
	Real hitEpsilon = 1e-4;

	Vec3 getColour(const Ray &r, uint bounces = 0) const;

public:
	Raymarch(const Scene &s, const Camera &cam, const Viewport &vp, Image &im);

	virtual void renderPixel(uint col, uint row) const override;

	// Number of full scene SDF evaluations before returning a miss
	void setMaxRayIterations(uint n) { maxRayIterations = n; }
	// Maximum number of times a ray is allowed to bounce off a surface
	void setMaxBounces(uint n) { maxBounces = n; }
	// Number of anti-aliasing multisample takes per pixel
	void setSamplesPerPixel(uint n) { samplesPerPixel = n; }
	// Ray length cutoff to prevent pointlessly hitting max iterations towards background
	void setMaxRayLength(Real l) { maxRayLength = l; }
	// Distance below which a hit is declared
	void setHitEpsilon(Real e) { hitEpsilon = e; }
};

Vec3 Raymarch::getColour(const Ray &r, uint bounces) const
{
	bool hit = false;
	HitRecord rec;
	Real dist = 0.0;
	uint iteration = 0;
	for (; iteration < maxRayIterations; iteration++)
	{
		Vec3 point = r.to(dist);
		rec.t = math::maxReal();
		hit = scene.hitWithSDF(point, hitEpsilon, rec);

		rec.t = math::abs(rec.t);
		dist += rec.t;

		if (hit || dist < rec.t || dist > maxRayLength)
			break;
	}

	if (hit)
	{
		Ray scattered;
		Vec3 attenuation;
		const Material *material = rec.hitable ? rec.hitable->getMaterial() : nullptr;
		Vec3 emission = material ? material->emitted(rec.point) : Vec3();

		if (bounces < maxBounces && material && material->scatter(r, rec, attenuation, scattered))
		{
			Vec3 pushNormal = dot(rec.normal, scattered.direction()) >= 0.0 ? rec.normal : -rec.normal;
			scattered = Ray(scattered.origin() + pushNormal * hitEpsilon, scattered.direction());
			return emission + getColour(scattered, bounces + 1) * attenuation;
		}
		else
		{
			return emission;
		}
	}

	return scene.background().sample(r.direction());
}

Raymarch::Raymarch(const Scene &s, const Camera &cam, const Viewport &vp, Image &im)
: PixelRenderer(vp)
, camera(cam)
, scene(s)
, image(im)
{}

void Raymarch::renderPixel(uint col, uint row) const
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
	Vec3 colour = getColour(r);

	for (uint i = 1; i < samplesPerPixel; i++)
	{
		u = Real(col + uniformRand()) * viewport.widthInv();
		v = Real(row + uniformRand()) * viewport.heightInv();
		r = camera.getRay(u, v);
		colour += getColour(r);
	}

	colour /= samplesPerPixel;
	colour = 255.99 * gammaCorrect(colour);

	Real colourArray[3];
	colourArray[0] = colour.r;
	colourArray[1] = colour.g;
	colourArray[2] = colour.b;
	image.store(int(col), int(row), (byte*)colourArray);
}

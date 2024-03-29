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

class Raytrace : public PixelRenderer
{
protected:
	const Camera &camera;
	const Scene &scene;
	Image &image;
	uint maxBounces = 50;
	uint samplesPerPixel = 100;

	Vec3 getColour(const Ray &r, uint bounces = 0) const;

public:
	Raytrace(const Scene &s, const Camera &cam, const Viewport &vp, Image &img);

	virtual void renderPixel(uint col, uint row) const override;

	// Maximum number of times a ray is allowed to bounce off a surface
	void setMaxBounces(uint n) { maxBounces = n; }
	// Number of anti-aliasing multisample takes per pixel
	void setSamplesPerPixel(uint n) { samplesPerPixel = n; }
};

// TODO: this is recursive, try iterative
Vec3 Raytrace::getColour(const Ray &r, uint bounces) const
{
	HitRecord rec;
	if (scene.hit(r, 0.001, math::maxReal(), rec))
	{
		Ray scattered;
		Vec3 attenuation;
		const Material *material = rec.hitable ? rec.hitable->getMaterial() : nullptr;
		Vec3 emission = material ? material->emitted(rec.point) : Vec3();
		if (bounces < maxBounces && material && material->scatter(r, rec, attenuation, scattered))
			return emission + getColour(scattered, bounces + 1) * attenuation;
		else
			return emission;
	}

	return scene.background().sample(r.direction());
}

Raytrace::Raytrace(const Scene &s, const Camera &cam, const Viewport &vp, Image &img)
: PixelRenderer(vp)
, camera(cam)
, scene(s)
, image(img)
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

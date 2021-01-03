#pragma once

#include "Common.hpp"

#include "Camera.hpp"
#include "Framebuffer.hpp"
#include "Material.hpp"
#include "Math.hpp"
#include "Random.hpp"
#include "Ray.hpp"
#include "Raytrace.hpp"
#include "Scene.hpp"
#include "Vec3.hpp"
#include "Viewport.hpp"

enum RaytraceVisualizerType
{
	RaytraceVisualizerTypeDepth,
	RaytraceVisualizerTypeNormal,
	RaytraceVisualizerTypeBounces
};

class RaytraceVisualizer : public Raytrace
{
private:
	RaytraceVisualizerType visualizerType;

	Vec3 getBounceColour(const Ray &r, uint bounces) const;

public:
	RaytraceVisualizer(RaytraceVisualizerType type, const Scene &s, const Camera &cam, const Viewport &vp, Framebuffer &fb);

	virtual void renderPixel(uint col, uint row) const override;
};

Vec3 RaytraceVisualizer::getBounceColour(const Ray &r, uint bounces) const
{
	HitRecord rec;
	if (scene.hit(r, 0.001, maxReal(), rec))
	{
		Ray scattered;
		Vec3 attenuation;
		if (bounces < maxBounces && rec.material && rec.material->scatter(r, rec, attenuation, scattered))
			return getBounceColour(scattered, bounces + 1);
	}
	return Vec3(1, 1, 1) * (Real(bounces) / Real(maxBounces));
}

RaytraceVisualizer::RaytraceVisualizer(RaytraceVisualizerType type, const Scene &s, const Camera &cam, const Viewport &vp, Framebuffer &fb)
: Raytrace(s, cam, vp, fb)
, visualizerType(type)
{}

void RaytraceVisualizer::renderPixel(uint col, uint row) const
{
	Real u = Real(col) + 0.5;
	Real v = Real(row) + 0.5;
	u *= viewport.widthInv();
	v *= viewport.heightInv();
	bool useDepthOfField = false;
	Ray r = camera.getRay(u, v, useDepthOfField);

	Vec3 colour;
	switch(visualizerType)
	{
		case RaytraceVisualizerTypeDepth:
		{
			HitRecord rec;
			if (scene.hit(r, 0.001, maxReal(), rec))
				colour = Vec3(1, 1, 1) / (1.0 + rec.t);
			break;
		}
		case RaytraceVisualizerTypeNormal:
		{
			HitRecord rec;
			if (scene.hit(r, 0.001, maxReal(), rec))
				colour = rec.normal * 0.5 + 0.5;
			break;
		}
		case RaytraceVisualizerTypeBounces:
		{
			colour = getBounceColour(r, 0);
			colour = gammaCorrect(colour);
			break;
		}
		default:
			break;
	}
	colour = 255.99 * colour;

	Real colourArray[3];
	colourArray[0] = colour.r;
	colourArray[1] = colour.g;
	colourArray[2] = colour.b;
	framebuffer.store(int(col), int(row), (byte*)colourArray);
}

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

class Preview : public PixelRenderer
{
protected:
	const Camera &camera;
	const Scene &scene;
	Image &image;

	bool useFakeLight = false;
	Vec3 fakeLightDirection;
	Vec3 fakeLightColor{1.0, 1.0, 1.0};
	Vec3 fakeAmbientLight{0.4, 0.4, 0.4};

	Vec3 getColour(const Ray &r) const;

public:
	Preview(const Scene &s, const Camera &cam, const Viewport &vp, Image &img);

	virtual void renderPixel(uint col, uint row) const override;

	void setUseFakeLight(bool b) { useFakeLight = b; }
	void setFakeLightDirection(const Vec3 &d) { fakeLightDirection = -normalize(d); }
	void setFakeLightColor(const Vec3 &c) { fakeLightColor = c; }
	void setFakeAmbientLight(const Vec3 &l) { fakeAmbientLight = l; }
};

Vec3 Preview::getColour(const Ray &r) const
{
	HitRecord rec;
	if (scene.hit(r, 0.001, math::maxReal(), rec))
	{
		const Material *material = rec.hitable ? rec.hitable->getMaterial() : nullptr;
		Vec3 emission = material ? material->emitted(rec.point) : Vec3();
		Vec3 scattering;

		Ray scattered;
		Vec3 attenuation;
		if (material && material->scatter(r, rec, attenuation, scattered))
		{
			if (useFakeLight)
			{
				scattering += attenuation * fakeAmbientLight;
				scattering += attenuation * fakeLightColor * math::max(0.0, dot(scattered.direction(), fakeLightDirection));
			}
			else
			{
				scattering += attenuation * scene.background().sample(scattered.direction());
			}
		}

		return emission + scattering;
	}

	return scene.background().sample(r.direction());
}

Preview::Preview(const Scene &s, const Camera &cam, const Viewport &vp, Image &img)
: PixelRenderer(vp)
, camera(cam)
, scene(s)
, image(img)
{
	setFakeLightDirection(Vec3(-1, -1, 1));
}

void Preview::renderPixel(uint col, uint row) const
{
	Real u = Real(col) + 0.5;
	Real v = Real(row) + 0.5;
	u *= viewport.widthInv();
	v *= viewport.heightInv();
	Ray r = camera.getRay(u, v, false);
	Vec3 colour = getColour(r);

	u = Real(col + uniformRand()) * viewport.widthInv();
	v = Real(row + uniformRand()) * viewport.heightInv();
	r = camera.getRay(u, v, false);
	colour += getColour(r);
	colour = 255.99 * gammaCorrect(colour);

	Real colourArray[3];
	colourArray[0] = colour.r;
	colourArray[1] = colour.g;
	colourArray[2] = colour.b;
	image.store(int(col), int(row), (byte*)colourArray);
}

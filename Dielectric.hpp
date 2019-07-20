#pragma once

#include "Common.hpp"

#include "Material.hpp"
#include "Math.hpp"
#include "Random.hpp"
#include "Sampling.hpp"

class Dielectric : public Material
{
private:
	Real refractiveIndex;

public:
	Dielectric() { refractiveIndex = 1.5; }
	Dielectric(Real _refractiveIndex) { refractiveIndex = _refractiveIndex; }

	virtual bool scatter(const Ray &rIn, const HitRecord &hr, Vec3 &attenuation, Ray &scattered) const;
};

bool Dielectric::scatter(const Ray &rIn, const HitRecord &hr, Vec3 &attenuation, Ray &scattered) const
{
	attenuation = Vec3(1, 1, 1);

	Vec3 v = rIn.direction();
	Vec3 n;

	Real refractiveIndexRatio;
	Real vDotN = dot(v, hr.normal);
	if (vDotN > 0)
	{
		// Inside surface, flip normal
		n = -hr.normal;
		refractiveIndexRatio = refractiveIndex;
	}
	else
	{
		n = hr.normal;
		refractiveIndexRatio = 1.0 / refractiveIndex;
		vDotN = -vDotN;
	}

	Real reflectance = 1;
	Vec3 refracted;
	if (refract(v, n, refractiveIndexRatio, refracted))
		reflectance = schlick(vDotN, refractiveIndex);

	if (uniformRand() < reflectance)
	{
		Vec3 reflected = reflect(v, n);
		scattered = Ray(hr.point, reflected);
	}
	else
	{
		scattered = Ray(hr.point, refracted);
	}

	return true;
}

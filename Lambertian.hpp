#pragma once

#include "Common.hpp"

#include "Material.hpp"
#include "Math.hpp"

class Lambertian : public Material
{
private:
	Vec3 albedo;

public:
	Lambertian() { albedo = Vec3(0.5, 0.5, 0.5); }
	Lambertian(const Vec3 &_albedo) { albedo = _albedo; }

	virtual bool scatter(const Ray &rIn, const HitRecord &hr, Vec3 &attenuation, Ray &scattered) const;
};

bool Lambertian::scatter(const Ray &rIn, const HitRecord &hr, Vec3 &attenuation, Ray &scattered) const
{
	Vec3 lambertianOut = hr.normal + sampleUnitSphere();
	scattered = Ray(hr.point, lambertianOut);
	attenuation = albedo;
	return true;
}

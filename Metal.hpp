#pragma once

#include "Common.hpp"

#include "Material.hpp"
#include "Math.hpp"
#include "Sampling.hpp"

class Metal : public Material
{
private:
	Vec3 albedo;
	Real roughness;

public:
	Metal() { albedo = Vec3(0.5, 0.5, 0.5); roughness = 0; }
	Metal(const Vec3 &_albedo) : Metal(_albedo, 0) {}
	Metal(const Vec3 &_albedo, Real _roughness) { albedo = _albedo; roughness = clamp(_roughness, 0, 1); }

	virtual bool scatter(const Ray &rIn, const HitRecord &hr, Vec3 &attenuation, Ray &scattered) const;
};

bool Metal::scatter(const Ray &rIn, const HitRecord &hr, Vec3 &attenuation, Ray &scattered) const
{
	Vec3 reflected = reflect(rIn.direction(), hr.normal);
	if (roughness)
		reflected += roughness * sampleUnitSphere();
	scattered = Ray(hr.point, reflected);
	attenuation = albedo;
	return dot(reflected, hr.normal) > 0;
}

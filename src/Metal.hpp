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
	Metal(const Vec3 &_albedo, Real _roughness) { albedo = _albedo; roughness = math::clamp(_roughness, 0.0, 1.0); }

	virtual bool scatter(const Ray &rIn, const HitRecord &hr, Vec3 &attenuation, Ray &scattered) const;
};

bool Metal::scatter(const Ray &rIn, const HitRecord &hr, Vec3 &attenuation, Ray &scattered) const
{
	Vec3 reflected = reflect(rIn.direction(), hr.normal);
	if (roughness)
	{
		// Regenerate reflected rays that fall below the surface
		uint count = 0;
		Vec3 reflectedAttempt;
		do
		{
			reflectedAttempt = reflected + roughness * sampleUnitSphere();
			count++;
		}
		while (count < 10 && dot(hr.normal, reflectedAttempt) < 0.0);
		reflected = reflectedAttempt;
	}
	scattered = Ray(hr.point, reflected);
	attenuation = albedo;
	return dot(reflected, hr.normal) > 0;
}

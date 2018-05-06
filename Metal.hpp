#pragma once

#include "Common.hpp"

#include "Material.hpp"

class Metal : public Material
{
private:
	Vec3 albedo;

public:
	Metal() { albedo = Vec3(0.5, 0.5, 0.5); }
	Metal(const Vec3 &_albedo) { albedo = _albedo; }

	virtual bool scatter(const Ray &rIn, const HitRecord &hr, Vec3 &attenuation, Ray &scattered) const;
};

bool Metal::scatter(const Ray &rIn, const HitRecord &hr, Vec3 &attenuation, Ray &scattered) const
{
	Vec3 reflected = reflect(normalize(rIn.direction()), hr.normal);
	scattered = Ray(hr.point, reflected);
	attenuation = albedo;
	return dot(reflected, hr.normal) > 0;
}

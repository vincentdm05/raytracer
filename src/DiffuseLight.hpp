#pragma once

#include "Common.hpp"

#include "Material.hpp"
#include "Math.hpp"
#include "Sampling.hpp"

class DiffuseLight : public Material
{
private:
	Vec3 albedo;

public:
	DiffuseLight() { albedo = Vec3(1.0, 1.0, 1.0); }
	DiffuseLight(const Vec3 &_albedo) { albedo = _albedo; }

	virtual bool scatter(const Ray &rIn, const HitRecord &hr, Vec3 &attenuation, Ray &scattered) const override { return false; }
	virtual Vec3 emitted(const Vec3 &p) const override;
};

Vec3 DiffuseLight::emitted(const Vec3 &p) const
{
	return albedo;
}

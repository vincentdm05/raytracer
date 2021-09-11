#pragma once

#include "Common.hpp"

#include "ConstantTexture.hpp"
#include "Material.hpp"
#include "Math.hpp"
#include "Sampling.hpp"
#include "Texture.hpp"

class Lambertian : public Material
{
private:
	const Texture *texture = nullptr;
	bool ownedTexture = false;

public:
	Lambertian(const Vec3 &albedo);
	Lambertian(const Texture &_texture) { texture = &_texture; }
	~Lambertian();

	virtual bool scatter(const Ray &rIn, const HitRecord &hr, Vec3 &attenuation, Ray &scattered) const override;
};

Lambertian::Lambertian(const Vec3 &albedo)
{
	texture = new ConstantTexture(albedo);
	ownedTexture = true;
}

Lambertian::~Lambertian()
{
	if (ownedTexture)
		delete texture;
}

bool Lambertian::scatter(const Ray &rIn, const HitRecord &hr, Vec3 &attenuation, Ray &scattered) const
{
	Vec3 lambertianOut = hr.normal + sampleUnitSphere();
	scattered = Ray(hr.point, lambertianOut);
	attenuation = texture->sample(hr.point);
	return true;
}

#pragma once

#include "Common.hpp"

#include "ConstantTexture.hpp"
#include "Vec3.hpp"
#include "Texture.hpp"

class CheckerTexture : public Texture
{
private:
	const Texture *texture1 = nullptr;
	const Texture *texture2 = nullptr;
	Vec3 frequency;
	bool ownedTextures = false;

public:
	CheckerTexture(const Vec3 &albedo1, const Vec3 &albedo2, const Vec3 &_frequency = Vec3(1.0, 1.0, 1.0));
	CheckerTexture(const Texture &_texture1, const Texture &_texture2, const Vec3 &_frequency = Vec3(1.0, 1.0, 1.0));
	~CheckerTexture();

	virtual Vec3 sample(const Vec3& position) const override;
};

CheckerTexture::CheckerTexture(const Vec3 &albedo1, const Vec3 &albedo2, const Vec3 &_frequency)
{
	texture1 = new ConstantTexture(albedo1);
	texture2 = new ConstantTexture(albedo2);
	frequency = _frequency;
	ownedTextures = true;
}

CheckerTexture::CheckerTexture(const Texture &_texture1, const Texture &_texture2, const Vec3 &_frequency)
{
	texture1 = &_texture1;
	texture2 = &_texture2;
	frequency = _frequency;
}

CheckerTexture::~CheckerTexture()
{
	if (ownedTextures)
	{
		delete texture1;
		delete texture2;
	}
}

Vec3 CheckerTexture::sample(const Vec3& position) const
{
	if ((sin(frequency.x * position.x) * sin(frequency.y * position.y) * sin(frequency.z * position.z)) >= 0)
		return texture1->sample(position);
	else
		return texture2->sample(position);
}

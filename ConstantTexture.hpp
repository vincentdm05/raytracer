#pragma once

#include "Common.hpp"

#include "Vec3.hpp"
#include "Texture.hpp"

class ConstantTexture : public Texture
{
private:
	Vec3 albedo;

public:
	ConstantTexture(const Vec3& _albedo) { albedo = _albedo; }

	virtual Vec3 sample(const Vec3& position) const override { return albedo; }
};

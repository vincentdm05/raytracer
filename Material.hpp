#pragma once

#include "Common.hpp"

#include "Hitable.hpp"
#include "Ray.hpp"
#include "Vec3.hpp"

class Material
{
public:
	virtual ~Material() {}

	virtual bool scatter(const Ray &rIn, const HitRecord &hr, Vec3 &attenuation, Ray &scattered) const = 0;
	virtual Vec3 emitted(const Vec3 &p) const { return Vec3(); }
};

Real schlick(Real cosine, Real refractionIndex)
{
	// Assuming air is the outside medium
	const Real mediumRefractionIndex = 1;
	Real r0 = (mediumRefractionIndex - refractionIndex) / (mediumRefractionIndex + refractionIndex);
	r0 *= r0;
	return r0 + (1 - r0) * pow(1 - cosine, 5);
}

#pragma once

#include "Common.hpp"

#include "Hitable.hpp"
#include "Ray.hpp"
#include "Vec3.hpp"

class Material
{
public:
	virtual bool scatter(const Ray &rIn, const HitRecord &hr, Vec3 &attenuation, Ray &scattered) const = 0;
};

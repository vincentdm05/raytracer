#pragma once

#include "Common.hpp"

#include "Ray.hpp"
#include "Vec3.hpp"

class Material;

struct HitRecord
{
	Real t;
	Vec3 normal;
	Vec3 point;
	const Material *material;
};

class Hitable
{
public:
	virtual ~Hitable() {}

	virtual bool hit(const Ray &r, Real minDist, Real maxDist, HitRecord &rec) const = 0;
};

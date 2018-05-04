#pragma once

#include "Common.hpp"

#include "Ray.hpp"
#include "Vec3.hpp"

struct HitRecord
{
	Real t;
	Vec3 normal;
	Vec3 point;
};

class Hitable
{
public:
	virtual bool hit(const Ray &r, Real minDist, Real maxDist, HitRecord &rec) const = 0;
};

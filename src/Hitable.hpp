#pragma once

#include "Common.hpp"

#include "Ray.hpp"
#include "Transform.hpp"
#include "Vec3.hpp"

class Material;

struct HitRecord
{
	Real t = 0;
	Vec3 normal;
	Vec3 point;
	const Material *material = nullptr;
};

class Hitable
{
protected:
	Transform transform;
	const Material *material = nullptr;

public:
	virtual ~Hitable() {}

	virtual bool hit(const Ray &r, Real minDist, Real maxDist, HitRecord &rec) const = 0;
};

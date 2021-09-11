#pragma once

#include "Common.hpp"

#include "Math.hpp"
#include "Ray.hpp"
#include "Transform.hpp"
#include "Vec3.hpp"

class Hitable;
class Material;

struct HitRecord
{
	Real t = 0;
	Vec3 point;
	Vec3 normal;
	const Hitable *hitable = nullptr;
};

class Hitable
{
protected:
	Transform transform;
	const Material *material = nullptr;

public:
	virtual ~Hitable() {}

	const Material *getMaterial() const { return material; }

	virtual bool hit(const Ray &r, Real minDist, Real maxDist, HitRecord &rec) const = 0;
	virtual bool hitWithSDF(const Vec3 &point, Real epsilon, HitRecord &rec) const;
	virtual Real evaluateSDF(const Vec3 &point) const { return math::maxReal(); }
	virtual Vec3 evaluateNormalFromSDF(const Vec3 &point, Real epsilon) const;
};

bool Hitable::hitWithSDF(const Vec3 &point, Real epsilon, HitRecord &rec) const
{
	rec.t = evaluateSDF(point);
	if (math::abs(rec.t) <= epsilon)
	{
		rec.point = point;
		rec.normal = evaluateNormalFromSDF(point, epsilon * 0.1);
		rec.hitable = this;
		return true;
	}
	return false;
}

Vec3 Hitable::evaluateNormalFromSDF(const Vec3 &point, Real epsilon) const
{
	// Generic normal evaluation by gradient
	Vec3 normal;
	normal.x = evaluateSDF(point + Vec3(epsilon, 0, 0)) - evaluateSDF(point - Vec3(epsilon, 0, 0));
	normal.y = evaluateSDF(point + Vec3(0, epsilon, 0)) - evaluateSDF(point - Vec3(0, epsilon, 0));
	normal.z = evaluateSDF(point + Vec3(0, 0, epsilon)) - evaluateSDF(point - Vec3(0, 0, epsilon));
	return normalize(normal);
}

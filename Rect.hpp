#pragma once

#include "Common.hpp"

#include "Hitable.hpp"

class Material;

class Rect : public Hitable
{
private:
	Real halfWidth = 1.0;
	Real halfHeight = 1.0;

public:
	Rect(const Transform &t, Real width, Real height, const Material &_material);

	virtual bool hit(const Ray &r, Real minDist, Real maxDist, HitRecord &rec) const;
};

Rect::Rect(const Transform &t, Real width, Real height, const Material &_material)
{
	transform = t;
	halfWidth = width * 0.5;
	halfHeight = height * 0.5;
	material = &_material;
}

bool Rect::hit(const Ray &r, Real minDist, Real maxDist, HitRecord &rec) const
{
	Ray ray = transform.applyInverse(r);

	Real t = -ray.origin().z / ray.direction().z;
	if (t < minDist || t > maxDist)
		return false;

	Real x = ray.origin().x + t * ray.direction().x;
	if (x < -halfWidth || x > halfWidth)
		return false;

	Real y = ray.origin().y + t * ray.direction().y;
	if (y < -halfHeight || y > halfHeight)
		return false;

	rec.t = t * transform.scale();
	rec.point = r.to(rec.t);
	rec.normal = rotate(Vec3(0, 0, 1), transform.rotation());;
	rec.material = material;

	return true;
}

#pragma once

#include "Common.hpp"

#include "Hitable.hpp"

class Material;

class Sphere : public Hitable
{
public:
	Sphere() {}
	Sphere(const Vec3 &center, Real radius, const Material &_material)
	{
		transform.setTranslation(center);
		transform.setScale(radius);
		material = &_material;
	}

	virtual bool hit(const Ray &r, Real minDist, Real maxDist, HitRecord &rec) const override;
	virtual Real evaluateSDF(const Vec3 &point) const override;
	virtual Vec3 evaluateNormalFromSDF(const Vec3 &point, Real epsilon) const override;
};

bool Sphere::hit(const Ray &r, Real minDist, Real maxDist, HitRecord &rec) const
{
	Vec3 center = transform.translation();
	Real radius = transform.scale();
	Vec3 oc = r.origin() - center;
	Real a = dot(r.direction(), r.direction());
	Real b = dot(oc, r.direction());
	Real c = dot(oc, oc) - radius * radius;
	Real discriminant = b * b - a * c;
	bool hit = false;
	if (discriminant > 0.0)
	{
		discriminant = sqrt(discriminant);
		minDist = minDist * a + b;
		maxDist = maxDist * a + b;
		if (-discriminant > minDist && -discriminant < maxDist)
		{
			hit = true;
			discriminant = -discriminant;
		}
		else if (discriminant > minDist && discriminant < maxDist)
		{
			hit = true;
		}
		if (hit)
		{
			rec.t = (-b + discriminant) / a;
			rec.point = r.to(rec.t);
			rec.normal = (rec.point - center) / radius;
			rec.hitable = this;
		}
	}

	return hit;
}

Real Sphere::evaluateSDF(const Vec3 &point) const
{
	return (point - transform.translation()).length() - transform.scale();
}

Vec3 Sphere::evaluateNormalFromSDF(const Vec3 &point, Real epsilon) const
{
	return (point - transform.translation()) * transform.inverseScale();
}

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

	virtual bool hit(const Ray &r, Real minDist, Real maxDist, HitRecord &rec) const;
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
		// discriminant = sqrt(discriminant);
		// Real temp = (-b - discriminant) / a;
		// if (temp > minDist && temp < maxDist)
		// {
		// 	rec.t = temp;
		// 	rec.point = r.to(rec.t);
		// 	rec.normal = (rec.point - center) / radius;
		//	rec.material = material;
		// 	return true;
		// }
		// temp = (-b + discriminant) / a;
		// if (temp > minDist && temp < maxDist)
		// {
		// 	rec.t = temp;
		// 	rec.point = r.to(rec.t);
		// 	rec.normal = (rec.point - center) / radius;
		//	rec.material = material;
		// 	return true;
		// }

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
			rec.material = material;
		}
	}

	return hit;
}

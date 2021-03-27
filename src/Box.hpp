#pragma once

#include "Common.hpp"

#include "Hitable.hpp"

class Material;

class Box : public Hitable
{
private:
	Vec3 halfExtents;

public:
	Box() {}
	Box(const Transform &t, const Vec3 &extents, const Material &_material);

	virtual bool hit(const Ray &r, Real minDist, Real maxDist, HitRecord &rec) const;
};

Box::Box(const Transform &t, const Vec3 &extents, const Material &_material)
{
	transform = t;
	halfExtents = extents * 0.5;
	material = &_material;
}

// Efficient hit implementation from http://www.jcgt.org/published/0007/03/04/paper-lowres.pdf
bool Box::hit(const Ray &r, Real minDist, Real maxDist, HitRecord &rec) const
{
	// Transform the ray into box space
	Ray ray = transform.applyInverse(r);

	Real winding = (max(abs(ray.origin()) / halfExtents) < 1.0) ? -1 : 1;
	Vec3 sgn = -sign(ray.direction());

	// Distance to plane
	Vec3 d = halfExtents * winding * sgn - ray.origin();
	d /= ray.direction();

	if ((d.x >= 0.0) &&
		abs(ray.origin().y + ray.direction().y * d.x) < halfExtents.y &&
		abs(ray.origin().z + ray.direction().z * d.x) < halfExtents.z)
	{
		sgn = Vec3(sgn.x, 0.0, 0.0);
	}
	else if ((d.y >= 0.0) &&
		abs(ray.origin().z + ray.direction().z * d.y) < halfExtents.z &&
		abs(ray.origin().x + ray.direction().x * d.y) < halfExtents.x)
	{
		sgn = Vec3(0.0, sgn.y, 0.0);
	}
	else if ((d.z >= 0.0) &&
		abs(ray.origin().x + ray.direction().x * d.z) < halfExtents.x &&
		abs(ray.origin().y + ray.direction().y * d.z) < halfExtents.y)
	{
		sgn = Vec3(0.0, 0.0, sgn.z);
	}
	else
	{
		sgn = Vec3();
	}

	Real hitDistance = (sgn.x != 0) ? d.x : ((sgn.y != 0) ? d.y : d.z);
	hitDistance *= transform.scale();
	if (any(sgn) && hitDistance > minDist && hitDistance < maxDist)
	{
		rec.t = hitDistance;
		rec.point = r.to(rec.t);
		rec.normal = rotate(sgn, transform.rotation());
		rec.material = material;
		return true;
	}

	return false;
}

#pragma once

#include "Common.hpp"

#include "Ray.hpp"
#include "Vec3.hpp"

Real hitSphere(const Vec3 &center, Real radius, const Ray &r)
{
	Vec3 oc = r.origin() - center;
	Real a = dot(r.direction(), r.direction());
	Real b = 2.0 * dot(oc, r.direction());
	Real c = dot(oc, oc) - radius * radius;
	Real discriminant = b * b - 4.0 * a * c;
	if (discriminant < 0.0)
		return -1.0;
	else
		return (-b - sqrt(discriminant)) / (2.0 * a);
}

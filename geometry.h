#pragma once

#include "common.h"

#include "ray.h"
#include "vec3.h"

bool hitSphere(const vec3 &center, real_t radius, const ray &r)
{
	vec3 oc = r.origin() - center;
	real_t a = dot(r.direction(), r.direction());
	real_t b = 2.0 * dot(oc, r.direction());
	real_t c = dot(oc, oc) - radius * radius;
	return b * b - 4.0 * a * c > 0.0;
}

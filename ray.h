#pragma once

#include "common.h"

#include "vec3.h"

class ray
{
private:
	vec3 o;
	vec3 d;

public:
	ray() { o = vec3(); d = vec3(1, 0, 0); }
	ray(const vec3 &_o, const vec3 &_d) { o = _o; d = _d; }

	const vec3 &origin() const { return o; }
	const vec3 &direction() const { return d; }
	inline vec3 to(real_t t) const { return o + t * d; }
};

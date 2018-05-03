#pragma once

#include "common.h"

#include "vec3.h"

inline real_t min(real_t a, real_t b)
{
	return a < b ? a : b;
}

inline real_t max(real_t a, real_t b)
{
	return a > b ? a : b;
}

inline real_t clamp(real_t a, real_t lower, real_t upper)
{
	return min(upper, max(lower, a));
}

inline vec3 lerp(const vec3 &a, const vec3 &b, real_t t)
{
	t = clamp(t, 0, 1);
	return (1 - t) * a + t * b;
}

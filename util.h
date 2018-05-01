#pragma once

#include "vec3.h"

inline comp_t min(comp_t a, comp_t b)
{
	return a < b ? a : b;
}

inline comp_t max(comp_t a, comp_t b)
{
	return a > b ? a : b;
}

inline comp_t clamp(comp_t a, comp_t lower, comp_t upper)
{
	return min(upper, max(lower, a));
}

inline vec3 lerp(const vec3 &a, const vec3 &b, comp_t t)
{
	t = clamp(t, 0, 1);
	return (1 - t) * a + t * b;
}

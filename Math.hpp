#pragma once

#include "Common.hpp"

#include "Vec3.hpp"

#include <random>

inline Real min(Real a, Real b)
{
	return a < b ? a : b;
}

inline Real max(Real a, Real b)
{
	return a > b ? a : b;
}

inline Real clamp(Real a, Real lower, Real upper)
{
	return min(upper, max(lower, a));
}

inline Vec3 lerp(const Vec3 &a, const Vec3 &b, Real t)
{
	t = clamp(t, 0, 1);
	return (1 - t) * a + t * b;
}

inline Vec3 sampleUnitSphere()
{
	// Rejection method for sampling inside a unit sphere
	Vec3 v;
	do
	{
		v = 2.0 * Vec3(drand48(), drand48(), drand48()) - 1.0;
	}
	while(v.squaredLength() >= 1.0);

	return v;
}

inline Vec3 sampleUnitDisk()
{
	Vec3 v;
	do
	{
		v = 2.0 * Vec3(drand48(), drand48(), 0.0) - Vec3(1.0, 1.0, 0.0);
	}
	while(v.squaredLength() >= 1.0);

	return v;
}

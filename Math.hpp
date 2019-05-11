#pragma once

#include "Common.hpp"

#include "Vec3.hpp"

#include <random>

inline Real pi()
{
	const double pi_val = 3.14159265358979323846;
	return Real(pi_val);
}

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

inline Real uniformRand()
{
	static std::default_random_engine generator;
	static std::uniform_real_distribution<Real> distribution(0.0, 1.0);
	return distribution(generator);
}

inline Vec3 sampleUnitSphere()
{
	// Rejection method for sampling inside a unit sphere
	Vec3 v;
	do
	{
		v = 2.0 * Vec3(uniformRand(), uniformRand(), uniformRand()) - 1.0;
	}
	while(v.squaredLength() >= 1.0);

	return v;
}

inline Vec3 sampleUnitDisk()
{
	Vec3 v;
	do
	{
		v = 2.0 * Vec3(uniformRand(), uniformRand(), 0.0) - Vec3(1.0, 1.0, 0.0);
	}
	while(v.squaredLength() >= 1.0);

	return v;
}

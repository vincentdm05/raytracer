#pragma once

#include "Common.hpp"

#include "Random.hpp"
#include "Vec3.hpp"

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

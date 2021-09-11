#pragma once

#include "Vec3.hpp"

Vec3 gammaCorrect(const Vec3 &colour)
{
	// Gamma 2 correction
	return sqrt(colour);
}

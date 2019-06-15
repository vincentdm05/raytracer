#pragma once

#include "Common.hpp"

#include <limits>

inline Real pi()
{
	const double pi_val = 3.14159265358979323846;
	return Real(pi_val);
}

inline Real min()
{
	return std::numeric_limits<Real>::min();
}

inline Real max()
{
	return std::numeric_limits<Real>::max();
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

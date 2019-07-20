#pragma once

#include "Common.hpp"

#include <limits>

inline Real pi()
{
	const double pi_val = 3.14159265358979323846;
	return Real(pi_val);
}

inline Real minReal()
{
	return std::numeric_limits<Real>::min();
}

inline Real maxReal()
{
	return std::numeric_limits<Real>::max();
}

inline int min(int a, int b)
{
	return a < b ? a : b;
}

inline Real min(Real a, Real b)
{
	return a < b ? a : b;
}

inline int max(int a, int b)
{
	return a > b ? a : b;
}

inline Real max(Real a, Real b)
{
	return a > b ? a : b;
}

inline int clamp(int a, int lower, int upper)
{
	return min(upper, max(lower, a));
}

inline Real clamp(Real a, Real lower, Real upper)
{
	return min(upper, max(lower, a));
}

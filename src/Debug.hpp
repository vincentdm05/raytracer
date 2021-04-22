#pragma once

#include "Common.hpp"

#include <cassert>
#include <cstdlib>
#include <iostream>

template <typename ... Args>
void toCerr(Args&&... args)
{
	(void)(int[]){0, (void(std::cerr << std::forward<Args>(args)), 0)...};
	std::cerr << std::endl;
}	

#ifdef NDEBUG

#define assertVerbose(...)
#define assertEqual(...)
#define assertEqualWithTolerance(...)

#else	// NDEBUG

#define assertVerbose(cond, ...) \
	if (!(cond)) \
	{ \
		toCerr("Assertion failed: (", #cond, "), (", __VA_ARGS__, "), function ", __func__, ", file ", __FILE__, ", line ", __LINE__, "."); \
		abort(); \
	}

#define assertEqual(left, right) \
	if (!(left == right)) \
	{ \
		toCerr("Assertion failed: ", #left, " does not equal ", #right, " (", left, " != ", right, "), function ", __func__, ", file ", __FILE__, ", line ", __LINE__, "."); \
		abort(); \
	}

#define assertEqualWithTolerance(left, right, epsilon) \
	if (!closeEnough(left, right, epsilon)) \
	{ \
		toCerr("Assertion failed: ", #left, " does not equal ", #right, " (", left, " != ", right, "), function ", __func__, ", file ", __FILE__, ", line ", __LINE__, "."); \
		abort(); \
	}

#endif	// NDEBUG

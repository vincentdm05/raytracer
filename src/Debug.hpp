#pragma once

#include "Common.hpp"

#include <cassert>
#include <cstdlib>
#include <iostream>

template <typename Arg>
std::ostream &toCerr(Arg arg)
{
	std::cerr << arg;
	return std::cerr;
}

template <typename Arg, typename ... Args>
std::ostream &toCerr(Arg arg, Args&&... args)
{
	toCerr(arg);
	toCerr(std::forward<Args>(args)...);
	return std::cerr;
}	

#ifdef NDEBUG

#define assertVerbose(...)
#define assertEqual(...)
#define assertEqualWithTolerance(...)

#else	// NDEBUG

#define assertVerbose(cond, ...) \
	if (!(cond)) \
	{ \
		toCerr("Assertion failed: (", #cond, "), (", __VA_ARGS__, "), function ", __func__, ", file ", __FILE__, ", line ", __LINE__, ".") << std::endl; \
		abort(); \
	}

#define assertEqual(left, right) \
	if (!(left == right)) \
	{ \
		toCerr("Assertion failed: ", #left, " does not equal ", #right, " (", left, " != ", right, "), function ", __func__, ", file ", __FILE__, ", line ", __LINE__, ".") << std::endl; \
		abort(); \
	}

#define assertEqualWithTolerance(left, right, epsilon) \
	if (!closeEnough(left, right, epsilon)) \
	{ \
		toCerr("Assertion failed: ", #left, " does not equal ", #right, " (", left, " != ", right, "), function ", __func__, ", file ", __FILE__, ", line ", __LINE__, ".") << std::endl; \
		abort(); \
	}

#endif	// NDEBUG

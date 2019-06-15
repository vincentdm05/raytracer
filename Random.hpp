#pragma once

#include "Common.hpp"

#include <random>

inline Real uniformRand()
{
	static std::default_random_engine generator;
	static std::uniform_real_distribution<Real> distribution(0.0, 1.0);
	return distribution(generator);
}

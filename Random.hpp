#pragma once

#include "Common.hpp"

#include <random>

inline Real uniformRand()
{
	static thread_local std::default_random_engine engine;
	std::uniform_real_distribution<Real> distribution(0.0, 1.0);
	return distribution(engine);
}

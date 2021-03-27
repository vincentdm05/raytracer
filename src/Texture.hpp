#pragma once

#include "Common.hpp"

#include "Vec3.hpp"

class Texture
{
public:
	virtual ~Texture() {}

	virtual Vec3 sample(const Vec3& position) const = 0;
};

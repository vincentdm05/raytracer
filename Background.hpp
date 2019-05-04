#pragma once

#include "Common.hpp"

#include "Math.hpp"
#include "Vec3.hpp"

class Background
{
private:
	Vec3 bottom;
	Vec3 top;

public:
	Background() : bottom(1, 0, 0), top(0, 0, 1) {}
	Background(const Vec3 &_bottom, const Vec3 &_top) { bottom = _bottom; top = _top; }

	Background &operator=(const Background &other) { if (this != &other) { bottom = other.bottom; top = other.top; }; return *this; }

	Vec3 sample(const Vec3 &direction) const;
};

Vec3 Background::sample(const Vec3 &direction) const
{
	Real altitude = 0.5 * (direction.y + 1.0);
	return lerp(bottom, top, altitude);
}

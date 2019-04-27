#pragma once

#include "Common.hpp"

#include "Vec3.hpp"

class Ray
{
private:
	Vec3 o;
	Vec3 d;

public:
	Ray() { o = Vec3(); d = Vec3(0, 0, -1); }
	Ray(const Vec3 &_o, const Vec3 &_d) { o = _o; d = normalize(_d); }

	const Vec3 &origin() const { return o; }
	const Vec3 &direction() const { return d; }
	inline Vec3 to(Real t) const { return o + t * d; }
};

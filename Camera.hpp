#pragma once

#include "Common.hpp"

#include "Ray.hpp"
#include "Vec3.hpp"

class Camera
{
private:
	Vec3 bottomLeft;
	Vec3 horizontal;
	Vec3 vertical;
	Vec3 origin;

public:
	Camera(const Vec3 &_bottomLeft, const Vec3 &_horizontal, const Vec3 &_vertical, const Vec3 &_origin);
	
	Ray getRay(Real u, Real v) const;
};

Camera::Camera(const Vec3 &_bottomLeft, const Vec3 &_horizontal, const Vec3 &_vertical, const Vec3 &_origin)
{
	bottomLeft = _bottomLeft;
	horizontal = _horizontal;
	vertical = _vertical;
	origin = _origin;
}

Ray Camera::getRay(Real u, Real v) const
{
	return Ray(origin, bottomLeft + u * horizontal + v * vertical);
}

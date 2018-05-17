#pragma once

#include "Common.hpp"

#include "Ray.hpp"
#include "Vec3.hpp"

struct Viewport
{
	uint width;
	uint height;

	Viewport() { width = 1; height = 1; }
	Viewport(uint w, uint h) { width = w; height = h; }

	Viewport &operator=(const Viewport &other) { if (this != &other) { width = other.width; height = other.height; }; return *this; }
};

class Camera
{
private:
	Viewport viewport;
	Vec3 bottomLeft;
	Vec3 horizontal;
	Vec3 vertical;
	Vec3 position;

public:
	Camera(const Vec3 &_position, const Vec3 &_direction, const Vec3 &_up, Real fovY, const Viewport &_viewport);

	const Viewport &getViewport() const { return viewport; }
	
	Ray getRay(Real u, Real v) const;
};

Camera::Camera(const Vec3 &_position, const Vec3 &_direction, const Vec3 &_up, Real fovY, const Viewport &_viewport)
{
	position = _position;

	viewport = _viewport;
	Real aspect = Real(viewport.width) / viewport.height;

	Real theta = fovY * M_PI / 180.0;
	Real halfHeight = tan(theta * 0.5);
	Real halfWidth = halfHeight * aspect;

	Vec3 direction = normalize(_direction);
	Vec3 right = normalize(cross(direction, _up));
	Vec3 up = cross(right, direction);

	bottomLeft = - halfWidth * right - halfHeight * up + direction;
	horizontal = 2.0 * halfWidth * right;
	vertical = 2.0 * halfHeight * up;
}

Ray Camera::getRay(Real u, Real v) const
{
	return Ray(position, bottomLeft + u * horizontal + v * vertical);
}

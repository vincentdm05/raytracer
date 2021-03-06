#pragma once

#include "Common.hpp"

#include "Math.hpp"
#include "Ray.hpp"
#include "Sampling.hpp"
#include "Vec3.hpp"
#include "Viewport.hpp"

class Camera
{
private:
	Viewport viewport;
	Vec3 bottomLeft;
	Vec3 horizontal;
	Vec3 vertical;
	Vec3 position;
	Vec3 right;
	Vec3 up;
	Real lensRadius;
	bool depthOfFieldEnabled = true;

public:
	Camera(const Vec3 &_position, const Vec3 &_direction, const Vec3 &_up, Real fovY, const Viewport &_viewport, Real aperture, Real focus);
	
	Ray getRay(Real u, Real v, bool useDepthOfField = true) const;
	const Viewport &getViewport() const { return viewport; }
	void setDepthOfFieldEnabled(bool value) { depthOfFieldEnabled = value; }
};

Camera::Camera(const Vec3 &_position, const Vec3 &_direction, const Vec3 &_up, Real fovY, const Viewport &_viewport, Real aperture, Real focus)
{
	viewport = _viewport;
	position = _position;
	lensRadius = aperture * 0.5;

	Real theta = fovY * (math::pi() / 180.0);
	Real halfHeight = tan(theta * 0.5);
	Real halfWidth = halfHeight * viewport.aspectRatio();

	Vec3 direction = normalize(_direction);
	right = normalize(cross(direction, _up));
	up = cross(right, direction);

	bottomLeft = position + focus * (-halfWidth * right - halfHeight * up + direction);
	horizontal = 2.0 * halfWidth * focus * right;
	vertical = 2.0 * halfHeight * focus * up;
}

Ray Camera::getRay(Real u, Real v, bool useDepthOfField) const
{
	Vec3 start = position;
	if (depthOfFieldEnabled && useDepthOfField)
	{
		Vec3 sample = lensRadius * sampleUnitDisk();
		Vec3 offset = right * sample.x + up * sample.y;
		start += offset;
	}
	return Ray(start, bottomLeft + u * horizontal + v * vertical - start);
}

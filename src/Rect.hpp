#pragma once

#include "Common.hpp"

#include "Hitable.hpp"

class Material;

class Rect : public Hitable
{
private:
	Real halfWidth = 1.0;
	Real halfHeight = 1.0;

public:
	Rect(const Transform &t, Real width, Real height, const Material &_material);

	virtual bool hit(const Ray &r, Real minDist, Real maxDist, HitRecord &rec) const override;
	virtual Real evaluateSDF(const Vec3 &point) const override;
	virtual Vec3 evaluateNormalFromSDF(const Vec3 &point, Real epsilon) const override;
};

Rect::Rect(const Transform &t, Real width, Real height, const Material &_material)
{
	transform = t;
	halfWidth = width * 0.5;
	halfHeight = height * 0.5;
	material = &_material;
}

bool Rect::hit(const Ray &r, Real minDist, Real maxDist, HitRecord &rec) const
{
	Ray ray = transform.applyInverse(r);

	Real t = -ray.origin().z / ray.direction().z;
	if (t < minDist || t > maxDist)
		return false;

	Real x = ray.origin().x + t * ray.direction().x;
	if (x < -halfWidth || x > halfWidth)
		return false;

	Real y = ray.origin().y + t * ray.direction().y;
	if (y < -halfHeight || y > halfHeight)
		return false;

	rec.t = t * transform.scale();
	rec.point = r.to(rec.t);
	rec.normal = rotate(Vec3(0.0, 0.0, ray.direction().z > 0.0 ? -1.0 : 1.0), transform.rotation());
	rec.hitable = this;

	return true;
}

Real Rect::evaluateSDF(const Vec3 &point) const
{
	Vec3 p = transform.applyInverse(point);
	Vec3 a(-halfWidth, -halfHeight, 0.0);
	Vec3 b( halfWidth, -halfHeight, 0.0);
	Vec3 c( halfWidth,  halfHeight, 0.0);
	Vec3 d(-halfWidth,  halfHeight, 0.0);
	Vec3 ba = b - a; Vec3 pa = p - a;
	Vec3 cb = c - b; Vec3 pb = p - b;
	Vec3 dc = d - c; Vec3 pc = p - c;
	Vec3 ad = a - d; Vec3 pd = p - d;
	Vec3 nor = cross(ba, ad);

	return sqrt(
		(math::sign(dot(cross(ba, nor), pa)) +
		math::sign(dot(cross(cb, nor), pb)) +
		math::sign(dot(cross(dc, nor), pc)) +
		math::sign(dot(cross(ad, nor), pd)) < 3.0)
		?
		math::min(math::min(math::min(
		(ba * math::clamp(dot(ba, pa) / ba.squaredLength(), 0.0, 1.0) - pa).squaredLength(),
		(cb * math::clamp(dot(cb, pb) / cb.squaredLength(), 0.0, 1.0) - pb).squaredLength()),
		(dc * math::clamp(dot(dc, pc) / dc.squaredLength(), 0.0, 1.0) - pc).squaredLength()),
		(ad * math::clamp(dot(ad, pd) / ad.squaredLength(), 0.0, 1.0) - pd).squaredLength())
		:
		dot(nor, pa) * dot(nor, pa) / nor.squaredLength());
}

Vec3 Rect::evaluateNormalFromSDF(const Vec3 &point, Real epsilon) const
{
	return rotate(Vec3(0.0, 0.0, transform.applyInverse(point).z > 0.0 ? 1.0 : -1.0), transform.rotation());
}

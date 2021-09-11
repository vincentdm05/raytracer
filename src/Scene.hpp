#pragma once

#include "Common.hpp"

#include "Background.hpp"
#include "Hitable.hpp"

#include <vector>

class Scene : public Hitable
{
private:
	Background bg;
	std::vector<const Hitable *> hitables;

public:
	Scene() {}
	Scene(uint size) { hitables.reserve(size); }
	~Scene() { hitables.clear(); }

	virtual bool hit(const Ray &r, Real minDist, Real maxDist, HitRecord &rec) const override;
	virtual bool hitWithSDF(const Vec3 &point, Real epsilon, HitRecord &rec) const override;
	virtual Real evaluateSDF(const Vec3 &point) const override;

	void setBackground(const Background &_background) { bg = _background; }
	const Background &background() const { return bg; }
	void add(const Hitable &hitable) { hitables.push_back(&hitable); }
};

bool Scene::hit(const Ray &r, Real minDist, Real maxDist, HitRecord &rec) const
{
	bool hit = false;
	Real closestHit = maxDist;
	for (const Hitable *hitable : hitables)
	{
		HitRecord tmpRec;
		if (hitable->hit(r, minDist, closestHit, tmpRec))
		{
			hit = true;
			closestHit = tmpRec.t;
			rec = tmpRec;
		}
	}
	return hit;
}

bool Scene::hitWithSDF(const Vec3 &point, Real epsilon, HitRecord &rec) const
{
	for (const Hitable *hitable : hitables)
	{
		HitRecord tmpRec;
		if (hitable->hitWithSDF(point, epsilon, tmpRec))
		{
			rec = tmpRec;
			return true;
		}
		else if (math::abs(tmpRec.t) < math::abs(rec.t))
		{
			rec.t = tmpRec.t;
		}
	}
	return false;
}

Real Scene::evaluateSDF(const Vec3 &point) const
{
	Real minDist = math::maxReal();
	for (const Hitable *hitable : hitables)
		minDist = math::min(minDist, hitable->evaluateSDF(point));
	return minDist;
}

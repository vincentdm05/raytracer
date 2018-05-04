#pragma once

#include "Common.hpp"

#include "Hitable.hpp"

#include <vector>

class Scene : public Hitable
{
private:
	std::vector<const Hitable *> hitables;

public:
	Scene() {}
	Scene(uint size) { hitables.reserve(size); }
	~Scene() { hitables.clear(); }

	virtual bool hit(const Ray &r, Real minDist, Real maxDist, HitRecord &rec) const;

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

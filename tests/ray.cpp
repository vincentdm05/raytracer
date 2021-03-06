#include "Common.hpp"

#ifdef NDEBUG
#undef NDEBUG
#endif

#include "Debug.hpp"
#include "Ray.hpp"
#include "Vec3.hpp"

int main()
{
	Ray r;
	assert(r.origin() == Vec3());
	assert(r.direction() == Vec3(0, 0, -1));
	assert(r.to(5) == Vec3(0, 0, -5));

	Ray r1(Vec3(0, 0, 1), Vec3(0, 0, -1));
	assert(r1.origin() == Vec3(0, 0, 1));
	assert(r1.direction() == normalize(Vec3(0, 0, -1)));
	assert(r1.to(1) == Vec3());

	return 0;
}

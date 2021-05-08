#include "Common.hpp"

#ifdef NDEBUG
#undef NDEBUG
#endif

#include "Debug.hpp"
#include "Quat.hpp"
#include "Transform.hpp"
#include "Vec3.hpp"

int main(int argc, char const *argv[])
{
	// Constructors, accessors and setters
	Transform a0;
	assertEqual(a0.rotation(), Quat());
	assertEqual(a0.translation(), Vec3());
	assertEqual(a0.scale(), 1);
	Transform a1(Quat(1, Vec3(2, 3, 4)), Vec3(-1, 0, 1), 2.5);
	assertEqual(a1.rotation(), Quat(1, Vec3(2, 3, 4)));
	assertEqual(a1.translation(), Vec3(-1, 0, 1));
	assertEqual(a1.scale(), 2.5);
	Transform a2;
	a2.setRotation(Quat(1, Vec3(2, 3, 4)));
	assertEqual(a2.rotation(), Quat(1, Vec3(2, 3, 4)));
	a2.setTranslation(Vec3(-1, 0, 1));
	assertEqual(a2.translation(), Vec3(-1, 0, 1));
	a2.setScale(2.5);
	assertEqual(a2.scale(), 2.5);

	// Transformation ops
	Transform b0(axisAngleToQuat(Vec3(0, -1, 0), math::pi() * 0.5), Vec3(-1, 0, 0), 0.5);
	Transform b1(axisAngleToQuat(Vec3(0, 1, 0), math::pi() * 0.5), Vec3(0, 0, -2), 2.0);
	assertEqualWithTolerance(b0.inverse(), b1, 0.0001);
	assertEqualWithTolerance(b0.apply(Vec3(1, 0, 0)), Vec3(-1, 0, 0.5), 0.0001);
	assertEqualWithTolerance(b1.apply(Vec3(1, 0, 0)), Vec3(0, 0, -4), 0.0001);
	assertEqualWithTolerance(b0.inverse().apply(Vec3(1, 0, 0)), b0.applyInverse(Vec3(1, 0, 0)), 0.0001);
	assertEqualWithTolerance(b0.applyInverse(Vec3(1, 0, 0)), b1.apply(Vec3(1, 0, 0)), 0.0001);

	return 0;
}

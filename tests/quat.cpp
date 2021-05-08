#include "Common.hpp"

#ifdef NDEBUG
#undef NDEBUG
#endif

#include "Debug.hpp"
#include "Math.hpp"
#include "Quat.hpp"
#include "Vec3.hpp"

int main(int argc, char const *argv[])
{
	// Constructors and comparison ops
	Quat a0;
	assert(a0.w == 1 && a0.x == 0 && a0.y == 0 && a0.z == 0);
	Quat a1(1, Vec3(2, 3, 4));
	assert(a1.w == 1 && a1.x == 2 && a1.y == 3 && a1.z == 4);
	assert(a0 != a1);
	Quat a2(a1);
	assert(a2 == a1);
	Quat a3(Quat(1, Vec3(2, 3, 4)));
	assert(a3 == Quat(1, Vec3(2, 3, 4)));

	// Assignment ops
	Quat b0 = a1;
	assert(b0 == a1);
	Quat b1 = Quat(1, Vec3(2, 3, 4));
	assert(b1 == Quat(1, Vec3(2, 3, 4)));

	// Sign ops
	Quat c0(1, Vec3(2, 3, 4));
	assert(+c0 == Quat(1, Vec3(2, 3, 4)));
	assert(-c0 == Quat(-1, -Vec3(2, 3, 4)));

	// Subscript ops
	Quat d0(1, Vec3(2, 3, 4));
	assert(d0.w == d0[0] && d0.x == d0[1] && d0.y == d0[2] && d0.z == d0[3]);
	assert(d0[4] == d0[3]);
	d0[0] = 5;
	assert(d0.w == 5);

	// Arithmetic assignment ops
	Quat e0;
	e0 += Quat(1, Vec3());
	assert(e0 == Quat(2, Vec3()));
	e0 -= Quat(0, Vec3(1, 0, 0));
	assert(e0 == Quat(2, Vec3(-1, 0, 0)));
	// Quaternion representing a pi/2 rotation around the x axis
	Quat e1 = axisAngleToQuat(Vec3(1, 0, 0), math::pi() * 0.5);
	e1 *= e1;
	Real angle;
	Vec3 axis;
	e1.getAxisAngle(axis, angle);
	assertVerbose(axis.normalize() == Vec3(1, 0, 0), axis, " != ", Vec3(1, 0, 0));
	assertVerbose(closeEnough(angle, math::pi(), 0.0001), angle, " != ", math::pi());
	e1 /= e1;
	assertVerbose(e1 == Quat(), e1, " != ", Quat());
	Quat e2(1, Vec3(-1, 0, 1));
	e2 += 1;
	assertVerbose(e2 == Quat(2, Vec3(0, 1, 2)), e2);
	e2 -= 2;
	assertVerbose(e2 == Quat(0, Vec3(-2, -1, 0)), e2);
	e2 *= -2;
	assertVerbose(e2 == Quat(0, Vec3(4, 2, 0)), e2);
	e2 /= 2;
	assertVerbose(e2 == Quat(0, Vec3(2, 1, 0)), e2);

	// Mathematical properties
	Quat f0(1, Vec3(2, 3, 4));
	assertVerbose(f0.getConjugate() == Quat(1, -Vec3(2, 3, 4)), f0.getConjugate());
	assertVerbose(f0.conjugate() == Quat(1, -Vec3(2, 3, 4)), f0);
	Quat f1(-2, Vec3(2, -2, 2));
	assert(f1.squaredNorm() == 16);
	assert(f1.norm() == 4);
	assert(f1.normalized().norm() == 1);
	assert(f1.normalize().norm() == 1);
	Quat f2(1, Vec3(-1, 1, -1));
	assertVerbose(f2.reciprocal() == Quat(0.25, Vec3(0.25, -0.25, 0.25)), f2.reciprocal());
	assertVerbose(f2.reciprocate() == Quat(0.25, Vec3(0.25, -0.25, 0.25)), f2);

	// Arithmetic ops
	assert(Quat(0, Vec3(1, 2, 3)) + Quat(1, Vec3(-1, 0, -2)) == Quat(1, Vec3(0, 2, 1)));
	assert(Quat(0, Vec3(1, 2, 3)) - Quat(1, Vec3(-1, 0, -2)) == Quat(-1, Vec3(2, 2, 5)));
	Quat g0 = axisAngleToQuat(Vec3(1, 0, 0), math::pi() * 0.5);
	Quat g1 = axisAngleToQuat(Vec3(1, 0, 0), math::pi());
	Quat g2 = axisAngleToQuat(Vec3(1, 0, 0), math::pi() * 1.5);
	assertVerbose(closeEnough(g1 * g0, g2, 0.0001), g1 * g0, " != ", g2);
	assertVerbose(closeEnough(g1 / g0, g0, 0.0001), g1 / g0, " != ", g0);
	assert(Quat(0, Vec3(1, 2, 3)) + 2 == Quat(2, Vec3(3, 4, 5)));
	assert(Quat(0, Vec3(1, 2, 3)) - 2 == Quat(-2, Vec3(-1, 0, 1)));
	assert(Quat(0, Vec3(-1, 2, 3)) * -1 == Quat(0, Vec3(1, -2, -3)));
	assert(Quat(0, Vec3(1, -2, 4)) / 2 == Quat(0, Vec3(0.5, -1, 2)));
	assert(2 + Quat(0, Vec3(1, -2, -1)) == Quat(2, Vec3(3, 0, 1)));
	assert(2 - Quat(0, Vec3(-1, 2, 3)) == Quat(2, Vec3(3, 0, -1)));
	assert(2 * Quat(0, Vec3(-1, 2, 3)) == Quat(0, Vec3(-2, 4, 6)));
	assert(closeEnough(1 / g0, axisAngleToQuat(Vec3(1, 0, 0), -math::pi() * 0.5), 0.0001));

	// Free function mathematical properties
	assert(dot(Quat(1, Vec3(-1, 2, -2)), Quat(-2, Vec3(-1, 2, 1))) == 1);
	assert(conjugate(Quat(1, Vec3(2, -3, 4))) == Quat(1, Vec3(-2, 3, -4)));
	Quat h0(-1, Vec3(2, -3, 4));
	assert(normalize(h0) == h0.normalized());
	assertVerbose(closeEnough(reciprocate(h0), h0.reciprocal(), 0.0001), reciprocate(h0), " != ", h0.reciprocal());
	assert(closeEnough(axisAngleToQuat(Vec3(1, 0, 0), math::pi() * 0.5), Quat(0.7071067811882787, Vec3(0.7071067811848163, 0.0, 0.0)), 0.0001));
	Vec3 rotated = rotate(Vec3(1, 1, 0), axisAngleToQuat(Vec3(0, 0, 1), math::pi() * 0.5));
	assertVerbose(closeEnough(rotated, Vec3(-1, 1, 0), 0.0001), rotated);

	return 0;
}

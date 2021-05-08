#include "Common.hpp"

#ifdef NDEBUG
#undef NDEBUG
#endif

#include "Debug.hpp"
#include "Math.hpp"

int main()
{
	Real a = math::pi();
	assert(a == math::pi() && a == Real(3.14159265358979323846));
	Real b = math::minReal();
	assert(b == math::minReal() && b == std::numeric_limits<Real>::min());
	Real c = math::maxReal();
	assert(c == math::maxReal() && c == std::numeric_limits<Real>::max());

	assertEqual(math::min(uint(2), uint(1)), 1);
	assertEqual(math::min(int(2), int(1)), 1);
	assertEqual(math::min(Real(2), Real(1)), 1);

	assertEqual(math::max(uint(2), uint(1)), 2);
	assertEqual(math::max(int(2), int(1)), 2);
	assertEqual(math::max(Real(2), Real(1)), 2);

	assertEqual(math::abs(Real(1)), 1);
	assertEqual(math::abs(Real(-1)), 1);
	assertEqual(math::abs(Real(1.2)), Real(1.2));
	assertEqual(math::abs(Real(-1.2)), Real(1.2));

	assertEqual(math::clamp(uint(3), uint(1), uint(2)), 2);
	assertEqual(math::clamp(uint(-3), uint(1), uint(2)), 2);
	assertEqual(math::clamp(int(3), int(1), int(2)), 2);
	assertEqual(math::clamp(int(-3), int(1), int(2)), 1);
	assertEqual(math::clamp(Real(3.1), Real(1.1), Real(2.1)), Real(2.1));
	assertEqual(math::clamp(Real(-3), Real(1.1), Real(2.1)), Real(1.1));

	assertEqualWithTolerance(Real(1.1), Real(1.2), Real(0.11));
	assertEqualWithTolerance(Real(-1.1), Real(-1.2), Real(0.11));
	assertEqualWithTolerance(Real(-1.1), Real(1.1), Real(2.21));

	return 0;
}

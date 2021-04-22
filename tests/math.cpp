#include "Common.hpp"

#ifdef NDEBUG
#undef NDEBUG
#endif

#include "Debug.hpp"
#include "Math.hpp"

int main()
{
	Real a = pi();
	assert(a == pi() && a == Real(3.14159265358979323846));
	Real b = minReal();
	assert(b == minReal() && b == std::numeric_limits<Real>::min());
	Real c = maxReal();
	assert(c == maxReal() && c == std::numeric_limits<Real>::max());

	assertEqual(min(uint(2), uint(1)), 1);
	assertEqual(min(int(2), int(1)), 1);
	assertEqual(min(Real(2), Real(1)), 1);

	assertEqual(max(uint(2), uint(1)), 2);
	assertEqual(max(int(2), int(1)), 2);
	assertEqual(max(Real(2), Real(1)), 2);

	assertEqual(abs(Real(1)), 1);
	assertEqual(abs(Real(-1)), 1);
	assertEqual(abs(Real(1.2)), Real(1.2));
	assertEqual(abs(Real(-1.2)), Real(1.2));

	assertEqual(clamp(uint(3), uint(1), uint(2)), 2);
	assertEqual(clamp(uint(-3), uint(1), uint(2)), 2);
	assertEqual(clamp(int(3), int(1), int(2)), 2);
	assertEqual(clamp(int(-3), int(1), int(2)), 1);
	assertEqual(clamp(Real(3.1), Real(1.1), Real(2.1)), Real(2.1));
	assertEqual(clamp(Real(-3), Real(1.1), Real(2.1)), Real(1.1));

	assertEqualWithTolerance(Real(1.1), Real(1.2), Real(0.11));
	assertEqualWithTolerance(Real(-1.1), Real(-1.2), Real(0.11));
	assertEqualWithTolerance(Real(-1.1), Real(1.1), Real(2.21));

	return 0;
}

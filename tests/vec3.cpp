#include "../Common.hpp"

#include "../Vec3.hpp"

#include <assert.h>
#include <iostream>

int main()
{
	Vec3 a;
	assert(a[0] == 0 && a[1] == 0 && a[2] == 0);
	Vec3 b(1, 2, 3);
	assert(b[0] == b.x && b[1] == b.y && b[2] == b.z);
	assert(b[0] == 1 && b[1] == 2 && b[2] == 3);
	assert((-b)[0] == -1 && (-b)[1] == -2 && (-b)[2] == -3);
	assert(b[3] == 1);

	a += b;
	assert(a == b);
	a -= b;
	assert(a == Vec3());
	b *= Vec3(2, 1, 1);
	assert(b == Vec3(2, 2, 3));
	b /= Vec3(2, 1, 3);
	assert(b == Vec3(1, 2, 1));
	a += 1;
	assert(a == Vec3(1, 1, 1));
	a -= 2;
	assert(a == Vec3(-1, -1, -1));
	a *= -1;
	assert(a == Vec3(1, 1, 1));
	b /= 2;
	assert(b == Vec3(0.5, 1, 0.5));

	Vec3 c(2, 2, 1);
	assert(c.squaredLength() == 9);
	assert(c.length() == 3);
	c.normalize();
	assert(c.length() == 1);

	assert(a + b == Vec3(1.5, 2, 1.5));
	assert(a - b == Vec3(0.5, 0, 0.5));
	assert(a * b == Vec3(0.5, 1, 0.5));
	assert(a / b == Vec3(2, 1, 2));
	assert(b + 0.5 == Vec3(1, 1.5, 1));
	assert(b - 0.5 == Vec3(0, 0.5, 0));
	assert(b * 2 == Vec3(1, 2, 1));
	assert(b / 0.5 == Vec3(1, 2, 1));
	assert(a + 3.14 == 3.14 + a);
	assert(a - 3.14 == -(3.14 - a));
	assert(a * 3.14 == 3.14 * a);
	assert(a / 2 == 0.5 / a);

	assert(dot(c, c) == c.squaredLength());
	assert(cross(Vec3(1, 0, 0), Vec3(0, 1, 0)) == Vec3(0, 0, 1));
	assert(normalize(Vec3(4, 9, 16)).length() == 1);
	assert(sqrt(Vec3(4, 9, 16)) == Vec3(2, 3, 4));

	assert(reflect(Vec3(1, -1, 0), Vec3(0, 1, 0)) == Vec3(1, 1, 0));
	Vec3 d;
	assert(refract(Vec3(1, -1, 0), Vec3(0, 1, 0), 1, d));
	assert(d == Vec3(1, -1, 0));

	try
	{
		Vec3 z;
		z.normalize();
		assert(0 && "Zero vector normalization should have thrown an error.");
	}
	catch (const char *msg)
	{}

	try
	{
		Vec3 z;
		normalize(z);
		assert(0 && "Zero vector normalization should have thrown an error.");
	}
	catch (const char *msg)
	{}

	std::cout << "All tests passed!" << std::endl;

	return 0;
}

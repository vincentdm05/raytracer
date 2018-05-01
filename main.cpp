#include <iostream>

#include "vec3.h"

void printTestImage()
{
	int nx = 200;
	int ny = 100;
	std::cout << "P3\n" << nx << " " << ny << "\n255\n";
	for (int row = ny - 1; row >= 0; row--)
	{
		for (int col = 0; col < nx; col++)
		{
			vec3<> v(float(col) / nx, float(row) / ny, 0.2f);
			v *= 255.99;
			std::cout << int(v.r) << " " << int(v.g) << " " << int(v.b) << "\n";
		}
	}
}

void testVec3()
{
	vec3<> v(1, 2, 3);
	std::cout << v << std::endl;
	std::cout << v.length() << std::endl;
	std::cout << v.normalize() << std::endl;
	std::cout << v.length() << std::endl;

	vec3<> a;
	std::cout << +a << std::endl;
	std::cout << -a << std::endl;
	vec3<> b(1, 2, 3);
	std::cout << -b << std::endl;
	std::cout << a + b << std::endl;
	std::cout << a - b << std::endl;
	std::cout << a + 1 << std::endl;
	std::cout << 1 + a << std::endl;
	std::cout << b * 2 << std::endl;
	std::cout << 2 * b << std::endl;

	a += 1;
	std::cout << a << std::endl;
	std::cout << a * b << std::endl;
	std::cout << a / b << std::endl;
	a *= 2;
	std::cout << a << std::endl;
	std::cout << dot(a, b) << std::endl;
	std::cout << cross(vec3<>(1, 0, 0), vec3<>(0, 1, 0)) << std::endl;

	vec3<> c(4, 0, 0);
	std::cout << c << std::endl;
	std::cout << normalize(c) << std::endl;
	std::cout << c << std::endl;
	std::cout << c.normalize() << std::endl;
	std::cout << c << std::endl;
}

int main()
{
	// testVec3();
	printTestImage();

	return 0;
}

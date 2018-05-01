#include <iostream>

#include "ray.h"
#include "vec3.h"
#include "util.h"

vec3 bgColour(const ray &r)
{
	vec3 d = normalize(r.direction());
	float t = 0.5 * (d.y + 1.0);
	return lerp(vec3(1, 1, 1), vec3(0.2, 0.0, 0.1), t);
}

void printTestImage()
{
	vec3 bottomLeft(-2.0, -1.0, -1.0);
	vec3 horizontal(4.0, 0.0, 0.0);
	vec3 vertical(0.0, 2.0, 0.0);
	vec3 origin(0, 0, 0);

	int nx = 200;
	int ny = 100;
	std::cout << "P3\n" << nx << " " << ny << "\n255\n";
	for (int row = ny - 1; row >= 0; row--)
	{
		for (int col = 0; col < nx; col++)
		{
			float u = float(col) / nx;
			float v = float(row) / ny;
			ray r(origin, bottomLeft + u * horizontal + v * vertical);
			vec3 colour = bgColour(r);
			colour *= 255.99;
			std::cout << int(colour.r) << " " << int(colour.g) << " " << int(colour.b) << "\n";
		}
	}
}

void testVec3()
{
	vec3 v(1, 2, 3);
	std::cout << v << std::endl;
	std::cout << v.length() << std::endl;
	std::cout << v.normalize() << std::endl;
	std::cout << v.length() << std::endl;

	vec3 a;
	std::cout << +a << std::endl;
	std::cout << -a << std::endl;
	vec3 b(1, 2, 3);
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
	std::cout << cross(vec3(1, 0, 0), vec3(0, 1, 0)) << std::endl;

	vec3 c(4, 0, 0);
	std::cout << c << std::endl;
	std::cout << normalize(c) << std::endl;
	std::cout << c << std::endl;
	std::cout << c.normalize() << std::endl;
	std::cout << c << std::endl;
}

void testRay()
{
	ray r;
	std::cout << r.origin() << " " << r.direction() << std::endl;
	std::cout << r.to(5) << std::endl;

	ray r1(vec3(), vec3(1, 1, 0));
	std::cout << r1.origin() << " " << r1.direction() << std::endl;
	std::cout << r1.to(2) << std::endl;
}

int main()
{
	// testVec3();
	// testRay();
	printTestImage();

	return 0;
}

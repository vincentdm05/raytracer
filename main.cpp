#include "Common.hpp"

#include "Camera.hpp"
#include "Lambertian.hpp"
#include "Math.hpp"
#include "Metal.hpp"
#include "Ray.hpp"
#include "Scene.hpp"
#include "Sphere.hpp"
#include "Vec3.hpp"

#include <iostream>
#include <random>

Vec3 getColour(const Ray &r, const Scene &scene, int depth = 0)
{
	HitRecord rec;
	if (scene.hit(r, 0.001, MAXFLOAT, rec))
	{
		Ray scattered;
		Vec3 attenuation;
		if (depth < 50 && rec.material && rec.material->scatter(r, rec, attenuation, scattered))
			return getColour(scattered, scene, depth + 1) * attenuation;
		else
			return Vec3();
	}

	Vec3 d = normalize(r.direction());
	Real background = 0.5 * (d.y + 1.0);
	// return lerp(Vec3(1, 1, 1), Vec3(0.5, 0.7, 1.0), background);
	return lerp(Vec3(0.619, 1, 0.694), Vec3(1, 0.639, 0.619), background);
}

void printTestImage()
{
	Camera camera(Vec3(-2.0, -1.0, -1.0), Vec3(4.0, 0.0, 0.0), Vec3(0.0, 2.0, 0.0), Vec3(0, 0, 0));

	Lambertian material0(Vec3(0.8, 0.3, 0.3));
	Lambertian material1(Vec3(0.8, 0.8, 0.0));
	Metal material2(Vec3(0.8, 0.6, 0.2));
	Metal material3(Vec3(0.8, 0.8, 0.8));

	Sphere sphere(Vec3(0, 0, -1), 0.5, &material0);
	Sphere ground(Vec3(0, -100.5, -1), 100, &material1);
	Sphere metallicSphere0(Vec3(1, 0, -1), 0.5, &material2);
	Sphere metallicSphere1(Vec3(-1, 0, -1), 0.5, &material3);
	Scene scene;
	scene.add(sphere);
	scene.add(ground);
	scene.add(metallicSphere0);
	scene.add(metallicSphere1);

	int nx = 200;
	int ny = 100;
	int ns = 100;
	std::cout << "P3\n" << nx << " " << ny << "\n255\n";
	for (int row = ny - 1; row >= 0; row--)
	{
		for (int col = 0; col < nx; col++)
		{
			Vec3 colour;
			for (int s = 0; s < ns; s++)
			{
				Real u = (col + drand48()) / nx;
				Real v = (row + drand48()) / ny;
				Ray r = camera.getRay(u, v);
				colour += getColour(r, scene);
			}
			// gamma 2 correction
			colour = 255.99 * sqrt(colour / ns);
			std::cout << int(colour.r) << " " << int(colour.g) << " " << int(colour.b) << "\n";
		}
	}
}

void testVec3()
{
	Vec3 v(1, 2, 3);
	std::cout << v << std::endl;
	std::cout << v.length() << std::endl;
	std::cout << v.normalize() << std::endl;
	std::cout << v.length() << std::endl;

	Vec3 a;
	std::cout << +a << std::endl;
	std::cout << -a << std::endl;
	Vec3 b(1, 2, 3);
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
	std::cout << cross(Vec3(1, 0, 0), Vec3(0, 1, 0)) << std::endl;

	Vec3 c(4, 0, 0);
	std::cout << c << std::endl;
	std::cout << normalize(c) << std::endl;
	std::cout << c << std::endl;
	std::cout << c.normalize() << std::endl;
	std::cout << c << std::endl;
}

void testRay()
{
	Ray r;
	std::cout << r.origin() << " " << r.direction() << std::endl;
	std::cout << r.to(5) << std::endl;

	Ray r1(Vec3(), Vec3(1, 1, 0));
	std::cout << r1.origin() << " " << r1.direction() << std::endl;
	std::cout << r1.to(2) << std::endl;

	Camera camera(Vec3(-2.0, -1.0, -1.0), Vec3(4.0, 0.0, 0.0), Vec3(0.0, 2.0, 0.0), Vec3(0, 0, 0));
	Ray ray = camera.getRay(0.5, 0.5);
	std::cout << ray.origin() << " " << ray.direction() << std::endl;
	std::cout << ray.to(2) << std::endl;
}

int main()
{
	// testVec3();
	// testRay();
	printTestImage();

	return 0;
}

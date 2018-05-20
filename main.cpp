#include "Common.hpp"

#include "Camera.hpp"
#include "Dielectric.hpp"
#include "Lambertian.hpp"
#include "Math.hpp"
#include "Metal.hpp"
#include "Ray.hpp"
#include "Raytracer.hpp"
#include "Scene.hpp"
#include "Sphere.hpp"
#include "Vec3.hpp"

#include <iostream>

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

	Camera camera(Vec3(0, 0, 0), Vec3(0, 0, -1), Vec3(0, 1, 0), 90, Viewport(200, 100));
	Ray ray = camera.getRay(0.5, 0.5);
	std::cout << ray.origin() << " " << ray.direction() << std::endl;
	std::cout << ray.to(2) << std::endl;
}

void testSceneMaterials(const Raytracer &raytracer)
{
	Camera camera(Vec3(0, 0, 0), Vec3(0, 0, -1), Vec3(0, 1, 0), 90, Viewport(200, 100));

	Lambertian material0(Vec3(0.8, 0.3, 0.3));
	Metal material1(Vec3(0.8, 0.8, 0.0), 0.3);
	Metal material2(Vec3(0.8, 0.6, 0.2));
	Dielectric material3(1.5);

	Sphere sphere(Vec3(0, 0, -1), 0.5, material0);
	Sphere ground(Vec3(0, -100.5, -1), 100, material1);
	Sphere metallicSphere(Vec3(1, 0, -1), 0.5, material2);
	Sphere transparentSphere(Vec3(-1, 0, -1), 0.5, material3);
	Sphere transparentSphereHollow(Vec3(-1, 0, -1), -0.45, material3);

	Scene scene;
	scene.add(sphere);
	scene.add(ground);
	scene.add(metallicSphere);
	scene.add(transparentSphere);
	scene.add(transparentSphereHollow);

	raytracer.printImage(scene, camera);
}

void testScenePositionableCamera(const Raytracer &raytracer)
{
	Camera camera(Vec3(0, 0, 0), Vec3(0, 0, -1), Vec3(0, 1, 0), 90, Viewport(200, 100));

	Lambertian material0(Vec3(0, 0, 1));
	Lambertian material1(Vec3(1, 0, 0));

	Real R = cos(M_PI / 4.0);
	Sphere sphere0(Vec3(-R, 0, -1), R, material0);
	Sphere sphere1(Vec3(R, 0, -1), R, material1);

	Scene scene;
	scene.add(sphere0);
	scene.add(sphere1);

	raytracer.printImage(scene, camera);
}

void testSceneFinal(const Raytracer &raytracer)
{
	// Put the camera inside an inverted transparent sphere
	Camera camera(Vec3(-2.0, 2.0, 1.0), Vec3(1.0, -1.0, -1.0), Vec3(0.0, 1.0, 0.0), 90, Viewport(200, 100));
}

void testSceneRef(const Raytracer &raytracer)
{
	Camera camera(Vec3(-2.0, 2.0, 1.0), Vec3(1.0, -1.0, -1.0), Vec3(0.0, 1.0, 0.0), 90, Viewport(200, 100));

	Lambertian material0(Vec3(0.1, 0.2, 0.5));
	Lambertian material1(Vec3(0.8, 0.8, 0.0));
	Metal material2(Vec3(0.8, 0.6, 0.2));
	Dielectric material3(1.5);

	Sphere sphere0(Vec3(0, 0, -1), 0.5, material0);
	Sphere sphere1(Vec3(0, -100.5, -1), 100, material1);
	Sphere sphere2(Vec3(1, 0, -1), 0.5, material2);
	Sphere sphere3(Vec3(-1, 0, -1), 0.5, material3);
	Sphere sphere4(Vec3(-1, 0, -1), -0.45, material3);

	Scene scene;
	scene.add(sphere0);
	scene.add(sphere1);
	scene.add(sphere2);
	scene.add(sphere3);
	scene.add(sphere4);

	raytracer.printImage(scene, camera);
}

int main()
{
	Raytracer raytracer(Background(Vec3(1, 1, 1), Vec3(0.5, 0.7, 1.0)));
	// Raytracer raytracer(Background(Vec3(0.619, 1, 0.694), Vec3(1, 0.639, 0.619)));

	// testVec3();
	// testRay();
	// testSceneMaterials(raytracer);
	// testScenePositionableCamera(raytracer);
	testSceneRef(raytracer);

	return 0;
}

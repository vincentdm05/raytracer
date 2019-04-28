#include "Common.hpp"

#include "Background.hpp"
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

#include <chrono>
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

	Camera camera(Vec3(0, 0, 0), Vec3(0, 0, -1), Vec3(0, 1, 0), 90, Viewport(200, 100), 2.0, 2.0);
	Ray ray = camera.getRay(0.5, 0.5);
	std::cout << ray.origin() << " " << ray.direction() << std::endl;
	std::cout << ray.to(2) << std::endl;
}

void testSceneMaterials(const Raytracer &raytracer)
{
	Camera camera(Vec3(0, 0, 0), Vec3(0, 0, -1), Vec3(0, 1, 0), 90, Viewport(200, 100), 2.0, 2.0);

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
	scene.setBackground(Background(Vec3(1, 1, 1), Vec3(0.5, 0.7, 1.0)));
	scene.add(sphere);
	scene.add(ground);
	scene.add(metallicSphere);
	scene.add(transparentSphere);
	scene.add(transparentSphereHollow);

	raytracer.printImage(scene, camera);
}

void testScenePositionableCamera(const Raytracer &raytracer)
{
	Camera camera(Vec3(0, 0, 0), Vec3(0, 0, -1), Vec3(0, 1, 0), 90, Viewport(200, 100), 2.0, 2.0);

	Lambertian material0(Vec3(0, 0, 1));
	Lambertian material1(Vec3(1, 0, 0));

	Real R = cos(M_PI / 4.0);
	Sphere sphere0(Vec3(-R, 0, -1), R, material0);
	Sphere sphere1(Vec3(R, 0, -1), R, material1);

	Scene scene;
	scene.setBackground(Background(Vec3(0.619, 1, 0.694), Vec3(1, 0.639, 0.619)));
	scene.add(sphere0);
	scene.add(sphere1);

	raytracer.printImage(scene, camera);
}

void testSceneRef(const Raytracer &raytracer)
{
	Vec3 cameraPosition(3.0, 3.0, 2.0);
	Vec3 focusPosition(0, 0, -1);
	Vec3 focusDirection = focusPosition - cameraPosition;
	Camera camera(cameraPosition, focusDirection, Vec3(0.0, 1.0, 0.0), 20, Viewport(200, 100), 2.0, focusDirection.length());

	Lambertian material0(Vec3(0.1, 0.2, 0.5));
	Lambertian material1(Vec3(0.8, 0.8, 0.0));
	Metal material2(Vec3(0.8, 0.6, 0.2));
	Dielectric material3(1.5);

	Sphere sphere0(focusPosition, 0.5, material0);
	Sphere sphere1(Vec3(0, -100.5, -1), 100, material1);
	Sphere sphere2(Vec3(1, 0, -1), 0.5, material2);
	Sphere sphere3(Vec3(-1, 0, -1), 0.5, material3);
	Sphere sphere4(Vec3(-1, 0, -1), -0.45, material3);

	Scene scene;
	scene.setBackground(Background(Vec3(0.619, 1, 0.694), Vec3(1, 0.639, 0.619)));
	scene.add(sphere0);
	scene.add(sphere1);
	scene.add(sphere2);
	scene.add(sphere3);
	scene.add(sphere4);

	raytracer.printImage(scene, camera);
}

void testSceneRandom(const Raytracer &raytracer)
{
	Vec3 cameraPosition(13.0, 2.0, 3.0);
	Vec3 focusPosition(0, 0.5, 0);
	Vec3 focusDirection = focusPosition - cameraPosition;
	Camera camera(cameraPosition, focusDirection, Vec3(0, 1, 0), 20, Viewport(1024, 640), 0.25, focusDirection.length() - 4.0);

	int arenaDimensions[4] = { -11, 11, -11, 11 };
	int arenaSize = (arenaDimensions[1] - arenaDimensions[0]) * (arenaDimensions[3] - arenaDimensions[2]);

	std::vector<Material *> materials;
	materials.reserve(arenaSize + 5);
	std::vector<Hitable *> objects;
	objects.reserve(arenaSize + 5);

	materials.push_back(new Lambertian(Vec3(0.5, 0.5, 0.5)));
	objects.push_back(new Sphere(Vec3(0, -1000, 0), 1000, *materials[0]));

	int materialIndex = 1;
	for (int i = arenaDimensions[0]; i < arenaDimensions[1]; i++)
	{
		for (int j = arenaDimensions[2]; j < arenaDimensions[3]; j++)
		{
			Real materialChooser = drand48();
			if (materialChooser > 0.9)
			{
				materials.push_back(new Dielectric(1.2 + drand48() * 0.5));
			}
			else if (materialChooser > 0.6)
			{
				materials.push_back(new Metal(Vec3(drand48(), drand48(), drand48()), drand48()));
			}
			else
			{
				materials.push_back(new Lambertian(Vec3(drand48(), drand48(), drand48())));
			}
			Vec3 spherePosition(i + drand48() * 2.0 - 1.0, 0.2 + drand48() * 0.2, j + drand48() * 2.0 - 1.0);
			objects.push_back(new Sphere(spherePosition, 0.2, *materials[materialIndex++]));
		}
	}

	materials.push_back(new Lambertian(Vec3(0.0, 1.0, 0.32)));
	materials.push_back(new Metal(Vec3(0.7, 0.6, 0.5), 0));
	materials.push_back(new Dielectric(1.5));
	objects.push_back(new Sphere(Vec3(-4, 1, 0), 1, *materials[materialIndex++]));
	objects.push_back(new Sphere(Vec3(4, 1, 0), 1, *materials[materialIndex++]));
	objects.push_back(new Sphere(Vec3(0, 1, 0), 1, *materials[materialIndex++]));

	Scene scene;
	scene.setBackground(Background(Vec3(0.619, 1, 0.694), Vec3(1, 0.639, 0.619)));
	for (Hitable *hitable : objects)
		scene.add(*hitable);

	raytracer.printImage(scene, camera);

	for (Hitable *hitable : objects)
		delete hitable;
	objects.clear();
	for (Material *material : materials)
		delete material;
	materials.clear();
}

int main()
{
	Raytracer raytracer;

	// testVec3();
	// testRay();
	// testSceneMaterials(raytracer);
	// testScenePositionableCamera(raytracer);
	// testSceneRef(raytracer);
	testSceneRandom(raytracer);

	// raytracer.setOutputEnabled(false);
	// auto start = std::chrono::system_clock::now();
	// testSceneRandom(raytracer);
	// auto stop = std::chrono::system_clock::now();
	// std::chrono::milliseconds delta = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
	// std::cout << "Duration: " << delta.count() << "ms" << std::endl;

	return 0;
}

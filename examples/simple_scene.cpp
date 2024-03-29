#include "Common.hpp"

#include "Background.hpp"
#include "Camera.hpp"
#include "Dielectric.hpp"
#include "File.hpp"
#include "Image.hpp"
#include "Lambertian.hpp"
#include "Metal.hpp"
#include "Raymarch.hpp"
#include "Raytrace.hpp"
#include "Renderer.hpp"
#include "Scene.hpp"
#include "Sphere.hpp"
#include "Vec3.hpp"

int main(int argc, char *argv[])
{
	Viewport viewport(200, 100);
	ImageDesc imageDesc;
	imageDesc.width = viewport.width();
	imageDesc.height = viewport.height();
	imageDesc.format = ImageFormat::r32g32b32f;
	Image image(imageDesc);

	Vec3 cameraPosition(3.0, 3.0, 2.0);
	Vec3 focusPosition(0, 0, -1);
	Vec3 focusDirection = focusPosition - cameraPosition;
	Camera camera(cameraPosition, focusDirection, Vec3(0.0, 1.0, 0.0), 20, viewport, 2.0, focusDirection.length());

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

#if 1
	Raytrace raytrace(scene, camera, viewport, image);
	Renderer renderer;
	renderer.render(raytrace);
#else
	Raymarch raymarch(scene, camera, viewport, image);
	raymarch.setMaxRayIterations(200);
	raymarch.setHitEpsilon(0.001);
	raymarch.setSamplesPerPixel(4);
	Renderer renderer;
	renderer.render(raymarch);
#endif

	file::writePpm(argv[argc > 1 ? 1 : 0], image, 255);

	return 0;
}

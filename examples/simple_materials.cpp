#include "Common.hpp"

#include "Background.hpp"
#include "Camera.hpp"
#include "Dielectric.hpp"
#include "File.hpp"
#include "Image.hpp"
#include "Lambertian.hpp"
#include "Metal.hpp"
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
	Camera camera(Vec3(0, 0, 0), Vec3(0, 0, -1), Vec3(0, 1, 0), 90, viewport, 0.25, 0.8);

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

	Raytrace raytrace(scene, camera, viewport, image);
	Renderer renderer;
	renderer.render(raytrace);

	file::writePpm(argv[argc > 1 ? 1 : 0], image, 255);

	return 0;
}

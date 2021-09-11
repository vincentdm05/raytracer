#include "Common.hpp"

#include "Background.hpp"
#include "Box.hpp"
#include "Camera.hpp"
#include "File.hpp"
#include "Image.hpp"
#include "Lambertian.hpp"
#include "Quat.hpp"
#include "Raymarch.hpp"
#include "Raytrace.hpp"
#include "Renderer.hpp"
#include "Scene.hpp"
#include "Sphere.hpp"
#include "Transform.hpp"
#include "Vec3.hpp"

int main(int argc, char const *argv[])
{
	Vec3 cameraPosition(0.0, 0.0, 5.0);
	Vec3 focusPosition(0, 0, 0);
	Vec3 focusDirection = focusPosition - cameraPosition;
	Viewport viewport(512, 256);
	ImageDesc imageDesc;
	imageDesc.width = viewport.width();
	imageDesc.height = viewport.height();
	imageDesc.format = ImageFormat::r32g32b32f;
	Image image(imageDesc);
	Camera camera(cameraPosition, focusDirection, Vec3(0.0, 1.0, 0.0), 20, viewport, 0.0, focusDirection.length());

	Lambertian material0(Vec3(0.6, 0.6, 0.6));
	Lambertian material1(Vec3(0.7, 0.2, 0.1));
	Lambertian material2(Vec3(0.15, 0.7, 0.15));
	Lambertian material3(Vec3(0.1, 0.2, 0.7));

	Sphere sphere0(Vec3(0.0, -100.5, -3.0), 100, material0);
	Sphere sphere1(focusPosition + Vec3(-1.0, 0.0, 0.0), 0.5, material1);
	Sphere sphere2(focusPosition + Vec3(-0.8, 0.0, -2.0), 0.5, material2);
	Sphere sphere3(focusPosition + Vec3(-0.6, 0.0, -4.0), 0.5, material3);
	Transform t(axisAngleToQuat(Vec3(1.0, 1.0, 0.0), math::pi() * 0.25), focusPosition + Vec3(1.0, 0.0, 0.0), 0.6);
	Box box(t, Vec3(1.0, 1.0, 1.0), material3);

	Scene scene;
	scene.setBackground(Background(Vec3(0.8, 0.3, 0.1), Vec3(0.2, 0.7, 0.9)));
	scene.add(sphere0);
	scene.add(sphere1);
	scene.add(sphere2);
	scene.add(sphere3);
	scene.add(box);

#if 1
	Raytrace raytrace(scene, camera, viewport, image);
	Renderer renderer;
	renderer.render(raytrace);
#else
	Raymarch raymarch(scene, camera, viewport, image);
	raymarch.setMaxRayIterations(200);
	raymarch.setHitEpsilon(0.001);
	raymarch.setSamplesPerPixel(100);
	Renderer renderer;
	renderer.render(raymarch);
#endif

	file::writePpm(argv[argc > 1 ? 1 : 0], image, 255);

	return 0;
}

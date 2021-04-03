#include "Common.hpp"

#include "Background.hpp"
#include "Box.hpp"
#include "Camera.hpp"
#include "Dielectric.hpp"
#include "File.hpp"
#include "Framebuffer.hpp"
#include "Lambertian.hpp"
#include "Metal.hpp"
#include "Raytrace.hpp"
#include "Renderer.hpp"
#include "Scene.hpp"
#include "Sphere.hpp"
#include "Transform.hpp"
#include "Vec3.hpp"

int main(int argc, char *argv[])
{
	Viewport viewport(512, 256);
	FramebufferDesc fbDesc;
	fbDesc.width = viewport.width();
	fbDesc.height = viewport.height();
	fbDesc.format = FramebufferFormat::r32g32b32f;
	Framebuffer framebuffer(fbDesc);

	Vec3 cameraPosition(0.0, 0.0, 5.0);
	Vec3 focusPosition(0, 0, 0);
	Vec3 focusDirection = focusPosition - cameraPosition;
	Camera camera(cameraPosition, focusDirection, Vec3(0.0, 1.0, 0.0), 20, viewport, 0.2, focusDirection.length());
	camera.setDepthOfFieldEnabled(false);

	Metal groundMaterial(Vec3(0.6, 0.6, 0.6), 0.02);
	Box ground(Transform(Quat(), Vec3(0.0, -100.55, -3.0), 200), Vec3(1.0, 1.0, 1.0), groundMaterial);

	Dielectric material1(Vec3(0.9, 0.4, 0.2), 1.5);
	Lambertian material3(Vec3(0.1, 0.2, 0.7));
	Metal material2(Vec3(0.15, 0.7, 0.15), 0.0);
	Dielectric material4(Vec3(0.1, 0.9, 0.8), 1.8);
	Metal material5(Vec3(0.7, 0.2, 0.9), 0.0);
	Lambertian material6(Vec3(0.7, 0.9, 0.2));

	Sphere hitable1(focusPosition + Vec3(-1.0, 0.0, 0.0), 0.5, material1);
	Sphere hitable2(focusPosition + Vec3(-0.8, 0.0, -2.0), 0.5, material2);
	Sphere hitable3(focusPosition + Vec3(-0.6, 0.0, -4.0), 0.5, material3);
	Quat rot = axisAngleToQuat(Vec3(1.0, 1.0, 0.0), pi() * 0.25);
	Vec3 ext(1.0, 1.0, 1.0);
	Box hitable4(Transform(rot, focusPosition + Vec3(1.0, 0.0, 0.0), 0.6), ext, material4);
	Box hitable5(Transform(rot, focusPosition + Vec3(0.8, 0.0, -2.0), 0.6), ext, material5);
	Box hitable6(Transform(rot, focusPosition + Vec3(0.6, 0.0, -4.0), 0.6), ext, material6);

	Scene scene;
	scene.setBackground(Background(Vec3(0.8, 0.3, 0.1), Vec3(0.2, 0.7, 0.9)));
	scene.add(ground);
	scene.add(hitable1);
	scene.add(hitable2);
	scene.add(hitable3);
	scene.add(hitable4);
	scene.add(hitable5);
	scene.add(hitable6);

	Raytrace raytrace(scene, camera, viewport, framebuffer);
	Renderer renderer;
	renderer.render(raytrace);

	file::writePpm(argv[argc > 1 ? 1 : 0], framebuffer, 255);

	return 0;
}

#include "Common.hpp"

#include "Background.hpp"
#include "Box.hpp"
#include "Camera.hpp"
#include "Dielectric.hpp"
#include "File.hpp"
#include "Image.hpp"
#include "Metal.hpp"
#include "Raymarch.hpp"
#include "Raytrace.hpp"
#include "Renderer.hpp"
#include "Scene.hpp"
#include "Sphere.hpp"
#include "Transform.hpp"
#include "Vec3.hpp"

int main(int argc, char *argv[])
{
	uint height = 300;
	const Real aspectRatio = 16.0 / 10.0;
	Viewport viewport(height * aspectRatio, height);
	ImageDesc imageDesc;
	imageDesc.width = viewport.width();
	imageDesc.height = viewport.height();
	imageDesc.format = ImageFormat::r32g32b32f;
	Image image(imageDesc);

	Vec3 cameraPosition(0.0, 0.0, 5.0);
	Vec3 focusPosition(0, 0, 0);
	Vec3 focusDirection = focusPosition - cameraPosition;
	Camera camera(cameraPosition, focusDirection, Vec3(0.0, 1.0, 0.0), 20, viewport, 0.2, focusDirection.length());
	camera.setDepthOfFieldEnabled(false);

	Metal groundMaterial(Vec3(0.8, 0.8, 0.8), 0.1);
	Box ground(Transform(Quat(), Vec3(0.0, -100.52, 0.0), 200), Vec3(1.0, 1.0, 1.0), groundMaterial);

	Metal sphereMaterial(Vec3(1.0, 0.4, 0.2), 0.0);
	Dielectric boxMaterial(Vec3(0.2, 1.0, 0.8), 2.2);

	Sphere sphere(focusPosition + Vec3(0.0, 0.0, 4.4875), 0.02, sphereMaterial);
	Quat rot = axisAngleToQuat(Vec3(1.0, 0.0, 0.0), math::pi() * 0.25) * axisAngleToQuat(Vec3(0.0, 1.0, 0.0), math::pi() * 0.25);
	Box box(Transform(rot, focusPosition + Vec3(0.0, 0.0, -0.45), 0.6), Vec3(1.0, 1.0, 1.0), boxMaterial);

	Scene scene;
	scene.setBackground(Background(Vec3(0.8, 0.9, 0.1), Vec3(0.8, 0.1, 0.9)));
	scene.add(ground);
	scene.add(sphere);
	scene.add(box);

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

	file::writePpm(argv[argc > 1 ? 1 : 0], image);

	return 0;
}

#include "Common.hpp"

#include "Background.hpp"
#include "Box.hpp"
#include "Camera.hpp"
#include "Dielectric.hpp"
#include "DiffuseLight.hpp"
#include "File.hpp"
#include "Image.hpp"
#include "Metal.hpp"
#include "Lambertian.hpp"
#include "Raytrace.hpp"
#include "Rect.hpp"
#include "Renderer.hpp"
#include "Scene.hpp"
#include "Sphere.hpp"
#include "Transform.hpp"
#include "Vec3.hpp"

// 3 spherical lights with glass around some
void renderScene1(Image &image, const Viewport &viewport, uint samplesPerPixel)
{
	Vec3 cameraPosition(0.0, 0.0, 8.0);
	Vec3 focusPosition(0, 0, 0);
	Vec3 focusDirection = focusPosition - cameraPosition;
	Camera camera(cameraPosition, focusDirection, Vec3(0.0, 1.0, 0.0), 20, viewport, 0.2, focusDirection.length());
	camera.setDepthOfFieldEnabled(false);

	Lambertian groundMaterial(Vec3(0.6, 0.8, 0.6));
	Dielectric glassMaterial(Vec3(0.9, 0.9, 0.9), 1.5);
	DiffuseLight lightMaterial(Vec3(1.0, 0.8, 0.7));

	Scene scene;
	scene.setBackground(Background(Vec3(0.0, 0.0, 0.03), Vec3(0.03, 0.0, 0.0)));

	Box ground(Transform(Quat(), Vec3(0.0, -100.55, -3.0), 200), Vec3(1.0, 1.0, 1.0), groundMaterial);
	scene.add(ground);
	Sphere glassSphere1(focusPosition, 0.5, glassMaterial);
	scene.add(glassSphere1);
	Sphere inverseGlassSphere1(focusPosition, -0.4, glassMaterial);
	scene.add(inverseGlassSphere1);
	Sphere lightSphere1(focusPosition, 0.25, lightMaterial);
	scene.add(lightSphere1);
	Sphere lightSphere2(focusPosition + Vec3(-1.0, 0.0, 0.0), 0.25, lightMaterial);
	scene.add(lightSphere2);
	Sphere glassSphere2(focusPosition + Vec3(1.0, 0.0, 0.0), 0.5, glassMaterial);
	scene.add(glassSphere2);
	Sphere lightSphere3(focusPosition + Vec3(1.0, 0.0, 0.0), 0.17, lightMaterial);
	scene.add(lightSphere3);

	Raytrace raytrace(scene, camera, viewport, image);
	raytrace.setSamplesPerPixel(samplesPerPixel);
	Renderer renderer;
	renderer.render(raytrace);
}

// Hollow glass sphere and top light creating caustic on the floor
void renderScene2(Image &image, const Viewport &viewport, uint samplesPerPixel)
{
	Vec3 cameraPosition(0.0, 0.0, 5.0);
	Vec3 focusPosition(0, 0, 0);
	Vec3 focusDirection = focusPosition - cameraPosition;
	Camera camera(cameraPosition, focusDirection, Vec3(0.0, 1.0, 0.0), 20, viewport, 0.2, focusDirection.length());
	camera.setDepthOfFieldEnabled(false);

	Lambertian groundMaterial(Vec3(0.6, 0.8, 0.6));
	Dielectric glassMaterial(Vec3(0.9, 0.9, 0.9), 1.5);
	DiffuseLight lightMaterial(10.0 * Vec3(1.0, 0.8, 0.7));

	Scene scene;
	scene.setBackground(Background(Vec3(0.0, 0.0, 0.03), Vec3(0.03, 0.0, 0.0)));

	Box ground(Transform(Quat(), Vec3(0.0, -100.55, -3.0), 200), Vec3(1.0, 1.0, 1.0), groundMaterial);
	scene.add(ground);
	Sphere glassSphere1(focusPosition, 0.5, glassMaterial);
	scene.add(glassSphere1);
	Sphere inverseGlassSphere1(focusPosition, -0.18, glassMaterial);
	scene.add(inverseGlassSphere1);
	Sphere lightSphere1(focusPosition + Vec3(0.0, 1.5, 0.0), 0.2, lightMaterial);
	scene.add(lightSphere1);

	Raytrace raytrace(scene, camera, viewport, image);
	raytrace.setSamplesPerPixel(samplesPerPixel);
	Renderer renderer;
	renderer.render(raytrace);
}

// Metal sphere inside glass, with light shining from behind
void renderScene3(Image &image, const Viewport &viewport, uint samplesPerPixel)
{
	Vec3 cameraPosition(0.0, 0.0, 5.0);
	Vec3 focusPosition(0, 0, 0);
	Vec3 focusDirection = focusPosition - cameraPosition;
	Camera camera(cameraPosition, focusDirection, Vec3(0.0, 1.0, 0.0), 20, viewport, 0.2, focusDirection.length());
	camera.setDepthOfFieldEnabled(false);

	Metal groundMaterial(Vec3(0.6, 0.8, 0.6), 0.9);
	Dielectric glassMaterial(Vec3(0.9, 0.9, 0.9), 1.5);
	Metal metalMaterial(Vec3(0.9, 0.85, 0.9), 0.0);
	DiffuseLight lightMaterial(10.0 * Vec3(1.0, 0.8, 0.7));

	Scene scene;
	scene.setBackground(Background(Vec3(0.0, 0.0, 0.03), Vec3(0.03, 0.0, 0.0)));

	Sphere ground(focusPosition + Vec3(0.0, -100.55, 0.0), 100.0, groundMaterial);
	scene.add(ground);
	Sphere glassSphere1(focusPosition, 0.5, glassMaterial);
	scene.add(glassSphere1);
	Sphere metalSphere1(focusPosition, 0.3, metalMaterial);
	scene.add(metalSphere1);
	Sphere lightSphere1(focusPosition + Vec3(0.0, 0.0, -1.5), 0.2, lightMaterial);
	scene.add(lightSphere1);

	Raytrace raytrace(scene, camera, viewport, image);
	raytrace.setSamplesPerPixel(samplesPerPixel);
	Renderer renderer;
	renderer.render(raytrace);
}

// Glass with two bubbles
void renderScene4(Image &image, const Viewport &viewport, uint samplesPerPixel)
{
	Vec3 cameraPosition(0.0, 0.0, 5.0);
	Vec3 focusPosition(0, 0, 0);
	Vec3 focusDirection = focusPosition - cameraPosition;
	Camera camera(cameraPosition, focusDirection, Vec3(0.0, 1.0, 0.0), 20, viewport, 0.2, focusDirection.length());
	camera.setDepthOfFieldEnabled(false);

	Metal groundMaterial(Vec3(0.6, 0.8, 0.6), 0.9);
	Dielectric glassMaterial(Vec3(0.9, 0.9, 0.9), 1.5);
	DiffuseLight lightMaterial(Vec3(1.0, 0.8, 0.7));

	Scene scene;
	scene.setBackground(Background(Vec3(0.0, 0.0, 0.3), Vec3(0.3, 0.0, 0.0)));

	Sphere ground(focusPosition + Vec3(0.0, -100.55, 0.0), 100.0, groundMaterial);
	scene.add(ground);
	Sphere glassSphere1(focusPosition, 0.5, glassMaterial);
	scene.add(glassSphere1);
	Sphere inverseGlassSphere1(focusPosition + Vec3(0.0, 0.22, 0.0), -0.05, glassMaterial);
	scene.add(inverseGlassSphere1);
	Sphere inverseGlassSphere2(focusPosition + Vec3(0.0, -0.04, 0.0), -0.18, glassMaterial);
	scene.add(inverseGlassSphere2);
	Sphere lightSphere1(focusPosition + Vec3(1.0, 1.5, -1.0), 0.2, lightMaterial);
	scene.add(lightSphere1);

	Raytrace raytrace(scene, camera, viewport, image);
	raytrace.setSamplesPerPixel(samplesPerPixel);
	Renderer renderer;
	renderer.render(raytrace);
}

// 4 metal spheres in a pyramid, surrounded by a few lights
void renderScene5(Image &image, const Viewport &viewport, uint samplesPerPixel)
{
	Vec3 cameraPosition(0.0, 0.0, 5.0);
	Vec3 focusPosition(0, 0, 0);
	Vec3 focusDirection = focusPosition - cameraPosition;
	Camera camera(cameraPosition, focusDirection, Vec3(0.0, 1.0, 0.0), 20, viewport, 0.2, focusDirection.length());
	camera.setDepthOfFieldEnabled(false);

	Metal groundMaterial(Vec3(0.6, 0.8, 0.6), 0.9);
	Metal metalMaterial(Vec3(0.9, 0.85, 0.9), 0.0);
	DiffuseLight lightMaterial(Vec3(1.0, 0.9, 0.6));

	Scene scene;
	scene.setBackground(Background(0.01 * Vec3(1.0, 0.4, 0.0), 0.01 * Vec3(0.0, 0.6, 1.0)));

	Sphere ground(focusPosition + Vec3(0.0, -100.55, 0.0), 100.0, groundMaterial);
	scene.add(ground);
	Sphere metalSphere1(focusPosition + Vec3(-1.0, 0.0, -2.5), 0.5, metalMaterial);
	scene.add(metalSphere1);
	Sphere metalSphere2(focusPosition + Vec3(1.0, 0.0, -2.5), 0.5, metalMaterial);
	scene.add(metalSphere2);
	Sphere metalSphere3(focusPosition, 0.5, metalMaterial);
	scene.add(metalSphere3);
	Sphere metalSphere4(focusPosition + Vec3(0.0, 1.0, -1.5), 0.5, metalMaterial);
	scene.add(metalSphere4);
	Sphere lightSphere1(focusPosition + Vec3(0.0, 0.0, -1.5), 0.2, lightMaterial);
	scene.add(lightSphere1);
	Sphere lightSphere2(focusPosition + Vec3(-2.0, 0.0, 1.5), 0.2, lightMaterial);
	scene.add(lightSphere2);
	Sphere lightSphere3(focusPosition + Vec3(2.0, 0.0, 1.5), 0.2, lightMaterial);
	scene.add(lightSphere3);
	Sphere lightSphere4(focusPosition + Vec3(0.0, 0.0, 5.5), 0.2, lightMaterial);
	scene.add(lightSphere4);

	Raytrace raytrace(scene, camera, viewport, image);
	raytrace.setSamplesPerPixel(samplesPerPixel);
	Renderer renderer;
	renderer.render(raytrace);
}

// Metallic pyramid with embedded light
void renderScene6(Image &image, const Viewport &viewport, uint samplesPerPixel)
{
	Vec3 cameraPosition(1.0, 4.0, 8.0);
	Vec3 focusPosition(0, 0, 0);
	Vec3 focusDirection = focusPosition + Vec3(0.0, 0.35, 0.0) - cameraPosition;
	Camera camera(cameraPosition, focusDirection, Vec3(0.0, 1.0, 0.0), 20, viewport, 0.2, focusDirection.length());
	camera.setDepthOfFieldEnabled(false);

	Metal groundMaterial(Vec3(0.6, 0.8, 0.6), 0.9);
	Metal metalMaterial(Vec3(0.9, 0.85, 0.9), 0.0);
	DiffuseLight lightMaterial(Vec3(1.0, 0.9, 0.6));

	Scene scene;
	scene.setBackground(Background(Vec3(0.0, 0.0, 0.03), Vec3(0.03, 0.0, 0.0)));

	Sphere ground(focusPosition + Vec3(0.0, -100.55, 0.0), 100.0, groundMaterial);
	scene.add(ground);
	Sphere metalSphere1(focusPosition + Vec3(-0.5, 0.0, -sqrt(3.0) / 6.0), 0.5, metalMaterial);
	scene.add(metalSphere1);
	Sphere metalSphere2(focusPosition + Vec3(0.5, 0.0, -sqrt(3.0) / 6.0), 0.5, metalMaterial);
	scene.add(metalSphere2);
	Sphere metalSphere3(focusPosition + Vec3(0.0, 0.0, sqrt(3.0) / 3.0), 0.5, metalMaterial);
	scene.add(metalSphere3);
	Sphere metalSphere4(focusPosition + Vec3(0.0, sqrt(6.0) / 3.0, 0.0), 0.5, metalMaterial);
	scene.add(metalSphere4);
	Sphere lightSphere1(focusPosition + Vec3(0.0, sqrt(6.0) / 12.0, 0.0), sqrt(6.0) / 4.0 - 0.5, lightMaterial);
	scene.add(lightSphere1);
	Sphere lightSphere2(focusPosition + Vec3(-2.0, 3.0, 0.0), 0.5, lightMaterial);
	scene.add(lightSphere2);

	Raytrace raytrace(scene, camera, viewport, image);
	raytrace.setSamplesPerPixel(samplesPerPixel);
	Renderer renderer;
	renderer.render(raytrace);
}

void renderScene7(Image &image, const Viewport &viewport, uint samplesPerPixel)
{
	Vec3 cameraPosition(0.0, 0.0, 8.0);
	Vec3 focusPosition(0, 0, 0);
	Vec3 focusDirection = focusPosition - cameraPosition;
	Camera camera(cameraPosition, focusDirection, Vec3(0.0, 1.0, 0.0), 20, viewport, 0.2, focusDirection.length());
	camera.setDepthOfFieldEnabled(false);

	Lambertian groundMaterial(Vec3(0.6, 0.8, 0.6));
	Metal metalMaterial(Vec3(0.9, 0.85, 0.9), 0.0);
	Dielectric glassMaterial(Vec3(0.9, 0.9, 0.9), 1.5);
	DiffuseLight lightMaterial1(Vec3(1.0, 0.0, 0.2));
	DiffuseLight lightMaterial2(Vec3(0.2, 0.0, 1.0));
	DiffuseLight lightMaterial3(Vec3(0.8, 0.9, 0.7));

	Scene scene;
	scene.setBackground(Background(Vec3(0.0, 0.0, 0.03), Vec3(0.03, 0.0, 0.0)));

	Sphere ground(focusPosition + Vec3(0.0, -100.55, 0.0), 100.0, groundMaterial);
	scene.add(ground);
	Sphere metalSphere1(focusPosition + Vec3(0.0, 0.05, 0.0), 0.5, metalMaterial);
	scene.add(metalSphere1);
	Rect lightRect1(Transform(axisAngleToQuat(Vec3(0.0, 1.0, 0.0), pi() * 0.45), focusPosition + Vec3(-1.0, 0.0, 0.0), 1.0), 1.5, 1.5, lightMaterial1);
	scene.add(lightRect1);
	Rect lightRect2(Transform(axisAngleToQuat(Vec3(0.0, 1.0, 0.0), pi() * -0.45), focusPosition + Vec3(1.0, 0.0, 0.0), 1.0), 1.5, 1.5, lightMaterial2);
	scene.add(lightRect2);
	Rect lightRect3(Transform(Quat(), focusPosition + Vec3(0.0, 0.0, -1.0), 1.0), 1.5, 1.5, lightMaterial3);
	scene.add(lightRect3);

	Raytrace raytrace(scene, camera, viewport, image);
	raytrace.setSamplesPerPixel(samplesPerPixel);
	Renderer renderer;
	renderer.render(raytrace);
}

int main(int argc, char *argv[])
{
	uint samplesPerPixel = 100;
	uint height = 300;
	const Real aspectRatio = 16.0 / 10.0;
	Viewport viewport(height * aspectRatio, height);
	ImageDesc imageDesc;
	imageDesc.width = viewport.width();
	imageDesc.height = viewport.height();
	imageDesc.format = ImageFormat::r32g32b32f;
	Image image(imageDesc);

	// renderScene1(image, viewport, samplesPerPixel);
	// renderScene2(image, viewport, samplesPerPixel);
	// renderScene3(image, viewport, samplesPerPixel);
	// renderScene4(image, viewport, samplesPerPixel);
	// renderScene5(image, viewport, samplesPerPixel);
	// renderScene6(image, viewport, samplesPerPixel);
	renderScene7(image, viewport, samplesPerPixel);

	file::writePpm(argv[argc > 1 ? 1 : 0], image);

	return 0;
}

#include "Common.hpp"

#include "Box.hpp"
#include "Camera.hpp"
#include "DiffuseLight.hpp"
#include "File.hpp"
#include "Image.hpp"
#include "Lambertian.hpp"
#include "Metal.hpp"
#include "Preview.hpp"
#include "Raymarch.hpp"
#include "Raytrace.hpp"
#include "Renderer.hpp"
#include "Rect.hpp"
#include "Scene.hpp"
#include "Transform.hpp"
#include "Vec3.hpp"

int main(int argc, char *argv[])
{
	const uint height = 256;
	const Real aspectRatio = 1.0;
	Viewport viewport(height * aspectRatio, height);
	ImageDesc imageDesc;
	imageDesc.width = viewport.width();
	imageDesc.height = viewport.height();
	imageDesc.format = ImageFormat::r32g32b32f;
	Image image(imageDesc);

	Vec3 cameraPosition(278.0, 278.0, -800.0);
	Vec3 focusPosition(278.0, 278.0, 0.0);
	Vec3 focusDirection = focusPosition - cameraPosition;
	Camera camera(cameraPosition, focusDirection, Vec3(0.0, 1.0, 0.0), 40.0, viewport, 0.0, 10.0);

	Lambertian red(Vec3(0.65, 0.05, 0.05));
	Lambertian white(Vec3(0.73, 0.73, 0.73));
	Lambertian green(Vec3(0.12, 0.45, 0.15));
	DiffuseLight lightMaterial(Vec3(15.0, 15.0, 15.0));

	Scene scene;
	scene.setBackground(Background(Vec3(0.0, 0.0, 0.0), Vec3(0.0, 0.0, 0.0)));

	Rect redWall(Transform(axisAngleToQuat(Vec3(0.0, 1.0, 0.0), math::pi() * -0.5), Vec3(555.0, 277.5, 277.5), 1.0), 555.0, 555.0, red);
	scene.add(redWall);
	Rect greenWall(Transform(axisAngleToQuat(Vec3(0.0, 1.0, 0.0), math::pi() * 0.5), Vec3(0.0, 277.5, 277.5), 1.0), 555.0, 555.0, green);
	scene.add(greenWall);
	Rect ceilingLight(Transform(axisAngleToQuat(Vec3(1.0, 0.0, 0.0), math::pi() * -0.5), Vec3(278.0, 554.0, 279.5), 1.0), 130.0, 105.0, lightMaterial);
	scene.add(ceilingLight);
	Rect ceilingWall(Transform(axisAngleToQuat(Vec3(1.0, 0.0, 0.0), math::pi() * 0.5), Vec3(277.5, 555.0, 277.5), 1.0), 555.0, 555.0, white);
	scene.add(ceilingWall);
	Rect floorWall(Transform(axisAngleToQuat(Vec3(1.0, 0.0, 0.0), math::pi() * -0.5), Vec3(277.5, 0.0, 277.5), 1.0), 555.0, 555.0, white);
	scene.add(floorWall);
	Rect backWall(Transform(axisAngleToQuat(Vec3(1.0, 0.0, 0.0), math::pi()), Vec3(277.5, 277.5, 555.0), 1.0), 555.0, 555.0, white);
	scene.add(backWall);
	Box smallBox(Transform(axisAngleToQuat(Vec3(0.0, 1.0, 0.0), math::pi() / 180.0 * -18.0), Vec3(185.5, 82.5, 169.0), 1.0), Vec3(165.0, 165.0, 165.0), white);
	scene.add(smallBox);
	Box bigBox(Transform(axisAngleToQuat(Vec3(0.0, 1.0, 0.0), math::pi() / 180.0 * 15.0), Vec3(368.5, 165.0, 351.5), 1.0), Vec3(165.0, 330.0, 165.0), white);
	scene.add(bigBox);

	Preview preview(scene, camera, viewport, image);
	preview.setUseFakeLight(true);
	Raytrace raytrace(scene, camera, viewport, image);
	raytrace.setSamplesPerPixel(100);
	Raymarch raymarch(scene, camera, viewport, image);
	raymarch.setMaxRayLength(1500.0);
	raymarch.setMaxRayIterations(100);
	raymarch.setHitEpsilon(1);
	raymarch.setSamplesPerPixel(100);

	Renderer renderer;
	// renderer.render(preview);
	renderer.render(raytrace);
	// renderer.render(raymarch);

	file::writePpm(argv[argc > 1 ? 1 : 0], image);

	return 0;
}

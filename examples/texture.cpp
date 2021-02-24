#include "../Common.hpp"

#include "../Background.hpp"
#include "../Box.hpp"
#include "../Camera.hpp"
#include "../CheckerTexture.hpp"
#include "../DiffuseLight.hpp"
#include "../File.hpp"
#include "../Framebuffer.hpp"
#include "../Metal.hpp"
#include "../Lambertian.hpp"
#include "../Raytrace.hpp"
#include "../Rect.hpp"
#include "../Renderer.hpp"
#include "../Scene.hpp"
#include "../Sphere.hpp"
#include "../Transform.hpp"
#include "../Vec3.hpp"

// Textured checker floor with two facing mirrors and overhead rectangle light
void renderScene1(Framebuffer &framebuffer, const Viewport &viewport, uint samplesPerPixel)
{
	Vec3 cameraPosition(0.0, 0.0, 1.0);
	Vec3 focusPosition(0, 0, 0);
	Vec3 focusDirection = focusPosition - cameraPosition;
	Camera camera(cameraPosition, focusDirection, Vec3(0.0, 1.0, 0.0), 20, viewport, 0.2, focusDirection.length());
	camera.setDepthOfFieldEnabled(false);

	CheckerTexture checkerTexture1(Vec3(1.0, 1.0, 1.0), Vec3(0.0, 0.0, 0.0), 5.0 * Vec3(1.0, 0.1, 1.0));
	Lambertian groundMaterial(checkerTexture1);
	Metal metalMaterial(Vec3(1.0, 1.0, 1.0), 0.0);
	DiffuseLight lightMaterial(Vec3(1.0, 0.8, 0.7));

	Scene scene;
	scene.setBackground(Background(Vec3(0.0, 0.0, 0.03), Vec3(0.03, 0.0, 0.0)));

	Sphere ground(focusPosition + Vec3(0.0, -100.55, 0.0), 100.0, groundMaterial);
	scene.add(ground);
	Rect metalRectangle1(Transform(Quat(), focusPosition + Vec3(0.0, 0.0, -2.0), 1.0), 4.0, 1.07, metalMaterial);
	scene.add(metalRectangle1);
	Rect metalRectangle2(Transform(Quat(), focusPosition + Vec3(0.0, 0.0, 2.0), 1.0), 4.0, 1.07, metalMaterial);
	scene.add(metalRectangle2);
	Rect lightRect1(Transform(axisAngleToQuat(Vec3(1.0, 0.0, 0.0), pi() * 0.5), focusPosition + Vec3(0.0, 0.5, 0.0), 1.0), 3.0, 3.0, lightMaterial);
	scene.add(lightRect1);

	Raytrace raytrace(scene, camera, viewport, framebuffer);
	raytrace.setSamplesPerPixel(samplesPerPixel);
	Renderer renderer;
	renderer.render(raytrace);
}

// Wide cornell box with checker walls
void renderScene2(Framebuffer &framebuffer, const Viewport &viewport, Real aspectRatio, uint samplesPerPixel)
{
	Vec3 cameraPosition(278.0, 278.0, -800.0);
	Vec3 focusPosition(278.0, 278.0, 0.0);
	Vec3 focusDirection = focusPosition - cameraPosition;
	Camera camera(cameraPosition, focusDirection, Vec3(0.0, 1.0, 0.0), 40.0, viewport, 0.0, 10.0);

	CheckerTexture redCheckerTexture(Vec3(1.0, 1.0, 1.0), Vec3(0.65, 0.05, 0.05), 0.1 * Vec3(1.0, 1.0, 1.0));
	Lambertian red(redCheckerTexture);
	Lambertian white(Vec3(0.73, 0.73, 0.73));
	CheckerTexture greenCheckerTexture(Vec3(1.0, 1.0, 1.0), Vec3(0.12, 0.45, 0.15), 0.1 * Vec3(1.0, 1.0, 1.0));
	Lambertian green(greenCheckerTexture);
	Metal mirror(Vec3(1.0, 1.0, 1.0), 0.8);
	DiffuseLight lightMaterial(Vec3(1.0, 1.0, 1.0));

	Scene scene;
	scene.setBackground(Background(Vec3(0.0, 0.0, 0.0), Vec3(0.0, 0.0, 0.0)));

	const Real boxHeight = 555.0;
	const Real boxWidth = boxHeight * aspectRatio;
	Rect redWall(Transform(axisAngleToQuat(Vec3(0.0, 1.0, 0.0), pi() * -0.5), Vec3((boxWidth - boxHeight) * 0.5 + boxHeight, boxHeight * 0.5, boxHeight * 0.5), 1.0), boxHeight, boxHeight, red);
	scene.add(redWall);
	Rect greenWall(Transform(axisAngleToQuat(Vec3(0.0, 1.0, 0.0), pi() * 0.5), Vec3(-(boxWidth - boxHeight) * 0.5, boxHeight * 0.5, boxHeight * 0.5), 1.0), boxHeight, boxHeight, green);
	scene.add(greenWall);
	Rect ceilingWall(Transform(axisAngleToQuat(Vec3(1.0, 0.0, 0.0), pi() * 0.5), Vec3(boxHeight * 0.5, boxHeight, boxHeight * 0.5), 1.0), boxWidth, boxHeight, lightMaterial);
	scene.add(ceilingWall);
	Rect floorWall(Transform(axisAngleToQuat(Vec3(1.0, 0.0, 0.0), pi() * -0.5), Vec3(boxHeight * 0.5, 0.0, boxHeight * 0.5), 1.0), boxWidth, boxHeight, white);
	scene.add(floorWall);
	Rect backWall(Transform(axisAngleToQuat(Vec3(1.0, 0.0, 0.0), pi()), Vec3(boxHeight * 0.5, boxHeight * 0.5, boxHeight), 1.0), boxWidth, boxHeight, mirror);
	scene.add(backWall);
	Box smallBox(Transform(axisAngleToQuat(Vec3(0.0, 1.0, 0.0), pi() / 180.0 * -18.0), Vec3(185.5, 82.5, 169.0), 1.0), Vec3(165.0, 165.0, 165.0), white);
	scene.add(smallBox);
	Box bigBox(Transform(axisAngleToQuat(Vec3(0.0, 1.0, 0.0), pi() / 180.0 * 15.0), Vec3(368.5, 165.0, 351.5), 1.0), Vec3(165.0, 330.0, 165.0), white);
	scene.add(bigBox);

	Raytrace raytrace(scene, camera, viewport, framebuffer);
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
	FramebufferDesc fbDesc;
	fbDesc.width = viewport.width();
	fbDesc.height = viewport.height();
	fbDesc.format = FramebufferFormat::FBFormat_r32g32b32f;
	Framebuffer framebuffer(fbDesc);

	// renderScene1(framebuffer, viewport, samplesPerPixel);
	renderScene2(framebuffer, viewport, aspectRatio, samplesPerPixel);

	file::writePpm(argv[argc > 1 ? 1 : 0], framebuffer);

	return 0;
}

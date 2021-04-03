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
#include "Viewer.hpp"

#include <chrono>
#include <iostream>

class FileWriterCallback : public FinishCallbackFunctor
{
private:
	const char *filename = nullptr;
	const Image &image;
	std::chrono::high_resolution_clock::time_point timeStart;

public:
	FileWriterCallback(const char *_filename, const Image &_image)
	: filename(_filename)
	, image(_image)
	{
		timeStart = std::chrono::high_resolution_clock::now();
	}

	virtual void operator()() override
	{
		std::chrono::high_resolution_clock::time_point timeStop = std::chrono::high_resolution_clock::now();
		std::cout << "Render duration: " <<
		std::chrono::duration_cast<std::chrono::milliseconds>(timeStop - timeStart).count() / 1000.0 << "s" <<
		std::endl;
		file::writePpm(filename, image);
	}
};

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
	FileWriterCallback finishCallback(argv[argc > 1 ? 1 : 0], image);
	Renderer renderer;
	renderer.renderAsync(raytrace, RenderFunctionTiles);
	renderer.setFinishCallback(finishCallback);

	Viewer viewer(imageDesc.width, imageDesc.height, "viewer");
	if (!viewer.show(image))
		return 1;

	renderer.waitForFinish();

	return 0;
}

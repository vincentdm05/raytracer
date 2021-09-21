#include "Common.hpp"

#include "Background.hpp"
#include "Box.hpp"
#include "Camera.hpp"
#include "Dielectric.hpp"
#include "DiffuseLight.hpp"
#include "File.hpp"
#include "Image.hpp"
#include "Lambertian.hpp"
#include "Metal.hpp"
#include "Preview.hpp"
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
	Viewport viewport(1024, 640);
	ImageDesc imageDesc;
	imageDesc.width = viewport.width();
	imageDesc.height = viewport.height();
	imageDesc.format = ImageFormat::r32g32b32f;
	Image image(imageDesc);

	Vec3 cameraPosition(13.0, 2.0, 3.0);
	Vec3 focusPosition(0, 0.5, 0);
	Vec3 focusDirection = focusPosition - cameraPosition;
	Camera camera(cameraPosition, focusDirection, Vec3(0, 1, 0), 20, viewport, 0.25, focusDirection.length() - 4.0);

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
			Real materialChooser = uniformRand();
			if (materialChooser > 0.9)
			{
				materials.push_back(new Dielectric(1.2 + uniformRand() * 0.5));
			}
			else if (materialChooser > 0.6)
			{
				materials.push_back(new Metal(Vec3(uniformRand(), uniformRand(), uniformRand()), uniformRand()));
			}
			else
			{
				materials.push_back(new Lambertian(Vec3(uniformRand(), uniformRand(), uniformRand())));
			}
			Vec3 spherePosition(i + uniformRand() * 2.0 - 1.0, 0.2 + uniformRand() * 0.2, j + uniformRand() * 2.0 - 1.0);
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

	Preview preview(scene, camera, viewport, image);
	Raytrace raytrace(scene, camera, viewport, image);
	raytrace.setSamplesPerPixel(samplesPerPixel);
	FileWriterCallback finishCallback(argv[argc > 1 ? 1 : 0], image);
	Renderer renderer;
	// renderer.renderAsync(raytrace, RenderFunctionTiles);
	renderer.renderAsync(preview, RenderFunctionTiles);
	renderer.setFinishCallback(finishCallback);

	Viewer viewer(imageDesc.width, imageDesc.height, "viewer");
	if (!viewer.show(image))
		return 1;

	renderer.waitForFinish();

	for (Hitable *hitable : objects)
		delete hitable;
	objects.clear();
	for (Material *material : materials)
		delete material;
	materials.clear();

	return 0;
}

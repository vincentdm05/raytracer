#include "Common.hpp"

#include "Background.hpp"
#include "Camera.hpp"
#include "Dielectric.hpp"
#include "File.hpp"
#include "Image.hpp"
#include "Lambertian.hpp"
#include "Metal.hpp"
#include "Preview.hpp"
#include "Raymarch.hpp"
#include "Raytrace.hpp"
#include "Renderer.hpp"
#include "Scene.hpp"
#include "Sphere.hpp"
#include "Vec3.hpp"

int main(int argc, char *argv[])
{
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
	Raymarch raymarch(scene, camera, viewport, image);
	raymarch.setMaxRayIterations(200);
	raymarch.setHitEpsilon(0.001);
	raymarch.setSamplesPerPixel(1);

	Renderer renderer;
	// renderer.render(preview);
	renderer.render(raytrace);
	// renderer.render(raymarch);

	file::writePpm(argv[argc > 1 ? 1 : 0], image);

	for (Hitable *hitable : objects)
		delete hitable;
	objects.clear();
	for (Material *material : materials)
		delete material;
	materials.clear();

	return 0;
}

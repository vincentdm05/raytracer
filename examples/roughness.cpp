#include "Common.hpp"

#include "Background.hpp"
#include "Camera.hpp"
#include "File.hpp"
#include "Image.hpp"
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
	Viewport viewport(900, 110);
	ImageDesc imageDesc;
	imageDesc.width = viewport.width();
	imageDesc.height = viewport.height();
	imageDesc.format = ImageFormat::r32g32b32f;
	Image image(imageDesc);

	Vec3 cameraPosition(0.0, 1.0, 11.8);
	Vec3 focusPosition(0, 0, -1);
	Vec3 focusDirection = focusPosition - cameraPosition;
	Camera camera(cameraPosition, focusDirection, Vec3(0.0, 1.0, 0.0), 5.0, viewport, 0.2, focusDirection.length());

	Scene scene;
	scene.setBackground(Background(Vec3(0.619, 1, 0.694), Vec3(1, 0.639, 0.619)));

	Metal groundMaterial(Vec3(0.2, 0.8, 0.3), 1.0);
	Sphere groundSphere(Vec3(0.0, -100.5, 0.0), 100.0, groundMaterial);
	scene.add(groundSphere);

	uint nSpheres = 9;
	std::vector<Material *> materials;
	std::vector<Sphere *> spheres;
	for (unsigned int i = 0; i < nSpheres; i++)
	{
		materials.push_back(new Metal(Vec3(0.8, 0.8, 0.8), Real(i) / Real(nSpheres - 1)));
		spheres.push_back(new Sphere(Vec3(-(nSpheres / 2.0) + Real(i) + 0.5, 0.0, -1.0), 0.5, *materials.back()));
		scene.add(*spheres.back());
	}

	Preview preview(scene, camera, viewport, image);
	Raytrace raytrace(scene, camera, viewport, image);
	Raymarch raymarch(scene, camera, viewport, image);
	raymarch.setMaxRayIterations(200);
	raymarch.setHitEpsilon(0.001);
	raymarch.setSamplesPerPixel(50);

	Renderer renderer;
	// renderer.render(preview);
	renderer.render(raytrace);
	// renderer.render(raymarch);

	file::writePpm(argv[argc > 1 ? 1 : 0], image);

	for (Material *material : materials)
		delete material;
	materials.clear();
	for (Sphere *sphere : spheres)
		delete sphere;
	spheres.clear();

	return 0;
}

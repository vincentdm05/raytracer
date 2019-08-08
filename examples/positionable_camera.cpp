#include "../Common.hpp"

#include "../Background.hpp"
#include "../Camera.hpp"
#include "../Framebuffer.hpp"
#include "../Lambertian.hpp"
#include "../Raytracer.hpp"
#include "../Scene.hpp"
#include "../Sphere.hpp"
#include "../Vec3.hpp"

int main(int argc, char *argv[])
{
	Viewport viewport(200, 100);
	Framebuffer framebuffer(viewport);
	Camera camera(Vec3(0, 0, 1), Vec3(0, 0, -1), Vec3(0, 1, 0), 45, viewport, 0.5, 2.0);

	Lambertian material0(Vec3(0, 0, 1));
	Lambertian material1(Vec3(1, 0, 0));

	Real R = cos(pi() / 4.0);
	Sphere sphere0(Vec3(-R, 0, -1), R, material0);
	Sphere sphere1(Vec3(R, 0, -1), R, material1);

	Scene scene;
	scene.setBackground(Background(Vec3(0.619, 1, 0.694), Vec3(1, 0.639, 0.619)));
	scene.add(sphere0);
	scene.add(sphere1);

	Raytracer raytracer;
	raytracer.render(scene, camera, framebuffer);
	framebuffer.writePpm(argv[argc > 1 ? 1 : 0]);

	return 0;
}

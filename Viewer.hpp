#pragma once

#include "Common.hpp"

#include "gpu/GpuBackendOpenGL.hpp"
#include "platform/PlatformBackendGLFW.hpp"

#include <chrono>
#include <thread>

class Viewer
{
private:
	PlatformBackendGLFW platform;
	GpuBackendOpenGL gpu;

	void clampToAspectRatio(uint &width, uint &height, const Real aspectRatio) const;
	void getCenteredViewportOrigin(uint &x, uint &y, uint windowWidth, uint windowHeight, uint viewportWidth, uint viewportHeight) const;

public:
	Viewer(uint displayWidth, uint displayHeight, const std::string &displayName);

	bool show(const Framebuffer &image);
};

void Viewer::clampToAspectRatio(uint &width, uint &height, const Real aspectRatio) const
{
	Real framebufferAspectRatio = Real(width) / Real(height);
	if (aspectRatio > framebufferAspectRatio)
	{
		height = width / aspectRatio;
	}
	else
	{
		width = height * aspectRatio;
	}
}

void Viewer::getCenteredViewportOrigin(uint &x, uint &y, uint windowWidth, uint windowHeight, uint viewportWidth, uint viewportHeight) const
{
	// Assuming windowWidth >= viewportWidth and windowHeight >= viewportHeight
	x = (windowWidth - viewportWidth) / 2;
	y = (windowHeight - viewportHeight) / 2;
}


Viewer::Viewer(uint displayWidth, uint displayHeight, const std::string &displayName)
: platform(displayWidth, displayHeight, displayName)
{}

bool Viewer::show(const Framebuffer &image)
{
	if (!platform.init())
		return false;

	if (!gpu.init())
		return false;

	while (platform.isLive())
	{
		uint width, height;
		platform.getFramebufferSize(width, height);

		uint viewportWidth = width;
		uint viewportHeight = height;
		clampToAspectRatio(viewportWidth, viewportHeight, image.getAspectRatio());
		uint x = 0;
		uint y = 0;
		getCenteredViewportOrigin(x, y, width, height, viewportWidth, viewportHeight);

		// TODO: compute scene max on cpu, or
		// TODO: run a compute dispatch to find image max and write to constant buffer, then scale in another pass
		gpu.setViewport(x, y, viewportWidth, viewportHeight);
		gpu.updateDisplayImage(image);
		gpu.render();

		// Swap buffers and poll events
		platform.update();

		// Sleep to reduce cpu time usage
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	gpu.end();
	platform.end();

	return true;
}

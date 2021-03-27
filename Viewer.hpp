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
	void normalizeImage(Framebuffer &image) const;

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

void Viewer::normalizeImage(Framebuffer &image) const
{
	uint width = image.getWidth();
	uint height = image.getHeight();
	uint channelAmount = image.getChannelAmount();
	// Assuming image has primitive type float
	float stagingPixel[channelAmount];
	float imageMax = 0.0f;
	for (uint y = 0; y < height; y++)
	{
		for (uint x = 0; x < width; x++)
		{
			image.load(x, y, (byte*)stagingPixel);
			for (uint c = 0; c < channelAmount; c++)
			{
				imageMax = max(imageMax, stagingPixel[c]);
			}
		}
	}

	float factor = abs(imageMax) > 1e-6 ? (1.0 / imageMax) : 1.0;

	for (uint y = 0; y < height; y++)
	{
		for (uint x = 0; x < width; x++)
		{
			image.load(x, y, (byte*)stagingPixel);
			for (uint c = 0; c < channelAmount; c++)
			{
				stagingPixel[c] *= factor;
			}
			image.store(x, y, (byte*)stagingPixel);
		}
	}
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

	Framebuffer stagingImage(image.getDesc());

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

		// Update staging and normalize
		stagingImage = image;
		normalizeImage(stagingImage);

		gpu.setViewport(x, y, viewportWidth, viewportHeight);
		gpu.updateDisplayImage(stagingImage);
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

#pragma once

#include "Common.hpp"

#include "Math.hpp"
#include "PixelRenderer.hpp"
#include "Viewport.hpp"

#include <atomic>
#include <future>

class Renderer
{
private:
	void renderTile(const PixelRenderer &pixelRenderer, uint tileX, uint tileY, uint tileSize) const;
	void renderTiles(const PixelRenderer &pixelRenderer, std::atomic<uint> &counter) const;
	void renderPixels(const PixelRenderer &pixelRenderer, std::atomic<uint> &counter) const;

public:
	Renderer() {}

	void render(const PixelRenderer &pixelRenderer) const;
};

void Renderer::renderTile(const PixelRenderer &pixelRenderer, uint tileX, uint tileY, uint tileSize) const
{
	const Viewport &vp = pixelRenderer.getViewport();

	uint tileOffsetX = tileX * tileSize;
	uint tileOffsetY = tileY * tileSize;

	uint stopX = min(tileOffsetX + tileSize, vp.width());
	uint stopY = min(tileOffsetY + tileSize, vp.height());
	for (uint row = tileOffsetY; row < stopY; row++)
	{
		for (uint col = tileOffsetX; col < stopX; col++)
		{
			pixelRenderer.renderPixel(col, row);
		}
	}
}

void Renderer::renderTiles(const PixelRenderer &pixelRenderer, std::atomic<uint> &counter) const
{
	constexpr uint tileSize = 64;
	const Viewport &vp = pixelRenderer.getViewport();
	uint tilesX = (vp.width() + tileSize - 1) / tileSize;
	uint tilesY = (vp.height() + tileSize - 1) / tileSize;
	uint tileAmount = tilesX * tilesY;
	while (true)
	{
		uint tileIndex = counter++;
		if (tileIndex >= tileAmount)
			break;

		uint tileY = tileIndex / tilesX;
		uint tileX = tileIndex % tilesX;

		renderTile(pixelRenderer, tileX, tileY, tileSize);
	}
}

void Renderer::renderPixels(const PixelRenderer &pixelRenderer, std::atomic<uint> &counter) const
{
	const Viewport &vp = pixelRenderer.getViewport();
	uint pixelAmount = vp.width() * vp.height();
	while (true)
	{
		uint pixelIndex = counter++;
		if (pixelIndex >= pixelAmount)
			break;

		uint row = pixelIndex / vp.width();
		uint col = pixelIndex % vp.width();

		pixelRenderer.renderPixel(col, row);
	}
}

void Renderer::render(const PixelRenderer &pixelRenderer) const
{
	std::atomic<uint> renderCounter {0};

	const uint nThreads = max(std::thread::hardware_concurrency(), 1u) - 1u;
	std::future<void> futures[nThreads];
	for (uint i = 0; i < nThreads; i++)
		futures[i] = std::async(std::launch::async, &Renderer::renderTiles, this, std::ref(pixelRenderer), std::ref(renderCounter));

	renderTiles(pixelRenderer, renderCounter);

	for (uint i = 0; i < nThreads; i++)
		futures[i].get();
}

#pragma once

#include "Common.hpp"

#include "Math.hpp"
#include "PixelRenderer.hpp"
#include "Viewport.hpp"

#include <atomic>
#include <future>
#include <vector>

enum RenderFunctionType
{
	RenderFunctionPixels,
	RenderFunctionTiles
};

// This functor class can be inherited to define actions taken once rendering has ended
class FinishCallbackFunctor
{
public:
	virtual void operator()() = 0;
};

class Renderer
{
private:
	const uint tileSize = 64;
	uint nThreads = 1;
	std::vector<std::future<void>> futures;
	std::atomic<uint> renderCounter {0};
	void (Renderer::*renderFunction)(const PixelRenderer &pixelRenderer);
	FinishCallbackFunctor *finishCallback = nullptr;
	uint *indexToGridMap = nullptr;

	void renderTile(const PixelRenderer &pixelRenderer, uint tileX, uint tileY, uint tileSize) const;
	void renderTiles(const PixelRenderer &pixelRenderer);
	void renderPixels(const PixelRenderer &pixelRenderer);
	// The async call can only take a constant pointer to member function, so this serves as a wrapper
	void renderInternal(const PixelRenderer &pixelRenderer) { (this->*renderFunction)(pixelRenderer); }
	void indexToGrid(uint &gridX, uint &gridY, const uint index) const;
	void makeGrid(uint width, uint height);
	void init(const PixelRenderer &pixelRenderer, RenderFunctionType type);
	void finish();

public:
	Renderer();
	~Renderer();

	void render(const PixelRenderer &pixelRenderer, RenderFunctionType type = RenderFunctionTiles);
	void renderAsync(const PixelRenderer &pixelRenderer, RenderFunctionType type = RenderFunctionTiles);
	void waitForFinish();
	void setFinishCallback(FinishCallbackFunctor &callback) { finishCallback = &callback; }
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

void Renderer::renderTiles(const PixelRenderer &pixelRenderer)
{
	const Viewport &vp = pixelRenderer.getViewport();
	uint tilesX = (vp.width() + tileSize - 1) / tileSize;
	uint tilesY = (vp.height() + tileSize - 1) / tileSize;
	uint tileAmount = tilesX * tilesY;
	uint tileIndex = 0;
	while (true)
	{
		tileIndex = renderCounter++;
		if (tileIndex >= tileAmount)
			break;

		uint tileY = tileIndex / tilesX;
		uint tileX = tileIndex % tilesX;
		indexToGrid(tileX, tileY, tileIndex);

		renderTile(pixelRenderer, tileX, tileY, tileSize);
	}

	if (tileIndex - tileAmount == nThreads - 1)
		finish();
}

void Renderer::renderPixels(const PixelRenderer &pixelRenderer)
{
	const Viewport &vp = pixelRenderer.getViewport();
	uint pixelAmount = vp.width() * vp.height();
	uint pixelIndex = 0;
	while (true)
	{
		pixelIndex = renderCounter++;
		if (pixelIndex >= pixelAmount)
			break;

		uint row = pixelIndex / vp.width();
		uint col = pixelIndex % vp.width();
		indexToGrid(col, row, pixelIndex);

		pixelRenderer.renderPixel(col, row);
	}

	if (pixelIndex - pixelAmount == nThreads - 1)
		finish();
}

void Renderer::indexToGrid(uint &gridX, uint &gridY, const uint index) const
{
	uint gridXY = indexToGridMap[index];
	gridX = gridXY & 0xffff;
	gridY = (gridXY >> 16) & 0xffff;
}

void Renderer::makeGrid(uint width, uint height)
{
	uint gridSize = width * height;
	indexToGridMap = new uint[gridSize];
	uint mapIndex = 0;
#if 0
	// Border to border grid
	for (uint y = 0; y < height; y++)
	{
		for (uint x = 0; x < width; x++)
		{
			// Left to right, bottom to top
			// uint gridXY = ((y & 0xffff) << 16) | (x & 0xffff);
			// Left to right, top to bottom
			uint gridXY = (((height - 1 - y) & 0xffff) << 16) | (x & 0xffff);
			indexToGridMap[mapIndex++] = gridXY;
		}
	}
#else
	// Center spiral grid, starting left, then up
	const int leftBound = -int(width) / 2;
	const int rightBound = int(width + 1) / 2;
	const int topBound = int(height + 1) / 2;
	const int bottomBound = -int(height) / 2;
	int x = 0;
	int y = 0;
	int dx = 0;
	int dy = -1;
	int temp = 0;
	uint maxSize = max(width, height);
	maxSize *= maxSize;
	for (uint i = 0; i < maxSize; i++)
	{
		// Within bounds
		if (x >= leftBound && x < rightBound && y >= bottomBound && y < topBound)
		{
			uint gridXY = (((y - bottomBound) & 0xffff) << 16) | ((x - leftBound) & 0xffff);
			indexToGridMap[mapIndex++] = gridXY;
		}
		// Turn
		if (x == -y || (x > 0 && x == y) || (x < 0 && x + 1 == y))
		{
			temp = dy;
			dy = -dx;
			dx = temp;
		}
		// Move
		x += dx;
		y += dy;
	}
#endif
}

void Renderer::init(const PixelRenderer &pixelRenderer, RenderFunctionType type)
{
	const Viewport &vp = pixelRenderer.getViewport();

	switch (type)
	{
		case RenderFunctionPixels:
		{
			renderFunction = &Renderer::renderPixels;

			uint width = vp.width();
			uint height = vp.height();
			makeGrid(width, height);

			break;
		}
		case RenderFunctionTiles:
		default:
		{
			renderFunction = &Renderer::renderTiles;

			uint width = (vp.width() + tileSize - 1) / tileSize;
			uint height = (vp.height() + tileSize - 1) / tileSize;
			makeGrid(width, height);

			break;
		}
	}

	renderCounter = 0;
}

void Renderer::finish()
{
	if (finishCallback)
		(*finishCallback)();
	finishCallback = nullptr;

	delete[] indexToGridMap;
	indexToGridMap = nullptr;
}

Renderer::Renderer()
{
	nThreads = max(std::thread::hardware_concurrency(), 1u);
	futures.reserve(nThreads);
}

Renderer::~Renderer()
{
	waitForFinish();
}

void Renderer::render(const PixelRenderer &pixelRenderer, RenderFunctionType type)
{
	init(pixelRenderer, type);

	for (uint i = 0; i < nThreads - 1; i++)
		futures.push_back(std::async(std::launch::async, &Renderer::renderInternal, this, std::ref(pixelRenderer)));

	renderInternal(pixelRenderer);

	waitForFinish();
}

void Renderer::renderAsync(const PixelRenderer &pixelRenderer, RenderFunctionType type)
{
	init(pixelRenderer, type);

	for (uint i = 0; i < nThreads; i++)
		futures.push_back(std::async(std::launch::async, &Renderer::renderInternal, this, std::ref(pixelRenderer)));
}

void Renderer::waitForFinish()
{
	for (uint i = 0; i < futures.size(); i++)
		futures[i].get();

	futures.clear();
}

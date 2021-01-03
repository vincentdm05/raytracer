#pragma once

#include "Common.hpp"

#include "Viewport.hpp"

// Inherit this class to allow the Renderer to render individual pixels.
// Define prerecorded inputs and outputs for each inherited type.
class PixelRenderer
{
protected:
	Viewport viewport;

public:
	PixelRenderer(const Viewport &vp) { viewport = vp; }

	virtual void renderPixel(uint col, uint row) const = 0;
	const Viewport &getViewport() const { return viewport; }
};

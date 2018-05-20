#pragma once

#include "Common.hpp"

class Viewport
{
private:
	uint w;
	uint h;

public:
	Viewport() { w = 1; h = 1; }
	Viewport(uint width, uint height) { w = width; h = height; }
	
	Viewport &operator=(const Viewport &other) { if (this != &other) { w = other.w; h = other.h; }; return *this; }

	uint width() const { return w; }
	uint height() const { return h; }
	Real aspectRatio() const { return Real(w) / h; }
};

#pragma once

#include "Common.hpp"

class Viewport
{
private:
	uint w;
	uint h;
	Real wInv;
	Real hInv;

public:
	Viewport() { w = 1; h = 1; wInv = 1.0; hInv = 1.0; }
	Viewport(uint width, uint height) { w = width; wInv = Real(1.0) / w; h = height; hInv = Real(1.0) / h; }
	
	Viewport &operator=(const Viewport &other) { if (this != &other) { w = other.w; wInv = other.wInv; h = other.h; hInv = other.hInv; }; return *this; }

	uint width() const { return w; }
	uint height() const { return h; }
	Real widthInv() const { return wInv; }
	Real heightInv() const { return hInv; }
	Real aspectRatio() const { return Real(w) / h; }
};

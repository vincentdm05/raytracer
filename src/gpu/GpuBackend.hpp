#pragma once

#include "Common.hpp"

class Framebuffer;

class GpuBackend
{
public:
	GpuBackend() {}

	virtual bool init() = 0;
	virtual bool end() = 0;
	virtual bool render() = 0;

	virtual void updateDisplayImage(const Framebuffer &image) = 0;
	virtual void setViewport(int x, int y, uint width, uint height) = 0;
};

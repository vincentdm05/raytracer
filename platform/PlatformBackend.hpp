#pragma once

#include "Common.hpp"

class PlatformBackend
{
public:
	PlatformBackend() {}
	virtual ~PlatformBackend() {}

	virtual bool init() = 0;
	virtual bool end() = 0;
	virtual bool isLive() const = 0;
	virtual bool update() = 0;
	virtual void close() = 0;
	virtual void getFramebufferSize(uint &widthOut, uint &heightOut) = 0;
};

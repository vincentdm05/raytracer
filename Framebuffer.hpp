#pragma once

#include "Common.hpp"

#include "Math.hpp"

#include <cstring>	// For memcpy

enum FramebufferFormat
{
	// How to read:
	// FBFormat_AX[BY[CZ[DW]]]F
	// Where A, B, C and D are channel names (e.g. r, g, b or a)
	// X, Y, Z and W are channel lengths (e.g. 8, 16 or 32)
	// F is the underlying format. See below:
	// f: floating point
	// ui: unsigned integer
	// si: signed integer
	// un: unsigned normalized [0, 1]
	// sn: signed normalized [-1, 1]

	FBFormat_Invalid,

	// TODO: add support for more formats
	// 32 bits per channel
	// 1 channel
	FBFormat_r32f,
	FBFormat_r32ui,
	FBFormat_r32si,
	FBFormat_r32un,
	FBFormat_r32sn,

	// 3 channels
	FBFormat_r32g32b32f,
	FBFormat_r32g32b32ui,
	FBFormat_r32g32b32si,
	FBFormat_r32g32b32un,
	FBFormat_r32g32b32sn,
};

// Descriptor for the framebuffer construction
struct FramebufferDesc
{
	uint width = 1;
	uint height = 1;
	FramebufferFormat format = FBFormat_r32f;
};

class Framebuffer
{
private:
	uint width = 0;
	uint height = 0;
	Real aspectRatio = 0.0;
	uint pixelSizeInBytes = 0;
	uint dataSize = 0;
	FramebufferFormat format = FBFormat_Invalid;
	byte *data = nullptr;

	uint positionToIndex(int x, int y) const;
	uint channelAmount() const;
	uint getPixelSizeInBytes();

public:
	Framebuffer(const FramebufferDesc &desc);
	~Framebuffer() { delete[] data; }

	uint getWidth() const { return width; }
	uint getHeight() const { return height; }
	Real getAspectRatio() const { return aspectRatio; }
	const byte *getData() const { return data; }
	void store(int x, int y, const byte *in);
	void load(int x, int y, byte *out) const;
};

uint Framebuffer::positionToIndex(int x, int y) const
{
	// TODO: ordering, e.g. Morton
	x = clamp(x, 0, width - 1);
	y = clamp(y, 0, height - 1);
	return uint(x + y * width) * pixelSizeInBytes;
}

uint Framebuffer::channelAmount() const
{
	uint amount = 0;
	if (format > FBFormat_Invalid)
		amount += 1;
	if (format > FBFormat_r32sn)
		amount += 2;

	return amount;
}

uint Framebuffer::getPixelSizeInBytes()
{
	if (pixelSizeInBytes)
		return pixelSizeInBytes;

	if (format == FBFormat_r32f ||
		format == FBFormat_r32g32b32f)
	{
		pixelSizeInBytes = sizeof(float);
	}
	else if (format == FBFormat_r32ui ||
		format == FBFormat_r32g32b32ui ||
		format == FBFormat_r32un ||
		format == FBFormat_r32g32b32un)
	{
		pixelSizeInBytes = sizeof(uint);
	}
	else if (format == FBFormat_r32si ||
		format == FBFormat_r32g32b32si ||
		format == FBFormat_r32sn ||
		format == FBFormat_r32g32b32sn)
	{
		pixelSizeInBytes = sizeof(int);
	}

	pixelSizeInBytes *= channelAmount();

	return pixelSizeInBytes;
}

Framebuffer::Framebuffer(const FramebufferDesc &desc)
{
	width = max(desc.width, 1u);
	height = max(desc.height, 1u);
	aspectRatio = Real(width) / Real(height);
	format = desc.format;

	dataSize = getPixelSizeInBytes() * width * height;
	data = new byte[dataSize]();
}

void Framebuffer::store(int x, int y, const byte *in)
{
	uint index = positionToIndex(x, y);
	std::memcpy(&data[index], in, pixelSizeInBytes);
}

void Framebuffer::load(int x, int y, byte *out) const
{
	uint index = positionToIndex(x, y);
	std::memcpy(out, &data[index], pixelSizeInBytes);
}

#pragma once

#include "Common.hpp"

#include "Math.hpp"

#include <cstring>	// For memcpy

enum class FramebufferFormat
{
	// How to read:
	// FramebufferFormat::AX[BY[CZ[DW]]]F
	// Where A, B, C and D are channel names (e.g. r, g, b or a)
	// X, Y, Z and W are channel lengths (e.g. 8, 16 or 32)
	// F is the underlying format. See below:
	// f: floating point
	// ui: unsigned integer
	// si: signed integer
	// un: unsigned normalized [0, 1]
	// sn: signed normalized [-1, 1]

	Invalid,

	// 32 bits per channel
	// 1 channel
	r32f,
	r32ui,
	r32si,

	// 3 channels
	r32g32b32f,
	r32g32b32ui,
	r32g32b32si,

	Count
};

// Descriptor for the framebuffer construction
struct FramebufferDesc
{
	uint width = 1;
	uint height = 1;
	FramebufferFormat format = FramebufferFormat::r32f;

	bool operator==(const FramebufferDesc &other) const;
	bool operator!=(const FramebufferDesc &other) const { return !(*this == other); }
};

bool FramebufferDesc::operator==(const FramebufferDesc &other) const
{
	return width == other.width &&
		height == other.height &&
		format == other.format;
}

class Framebuffer
{
private:
	FramebufferDesc descriptor;
	byte *data = nullptr;

	// Meta data
	Real aspectRatio = 0.0;
	uint channelAmount = 0;
	uint pixelSizeInBytes = 0;
	uint dataSize = 0;

	uint positionToIndex(int x, int y) const;

public:
	Framebuffer(const FramebufferDesc &desc);
	~Framebuffer() { delete[] data; }

	void operator=(const Framebuffer &other);

	FramebufferDesc getDesc() const { return descriptor; }
	uint getWidth() const { return descriptor.width; }
	uint getHeight() const { return descriptor.height; }
	Real getAspectRatio() const { return aspectRatio; }
	uint getChannelAmount() const { return channelAmount; }
	uint getPixelSizeInBytes() const { return pixelSizeInBytes; }
	const byte *getData() const { return data; }
	void store(int x, int y, const byte *in);
	void load(int x, int y, byte *out) const;
};

uint Framebuffer::positionToIndex(int x, int y) const
{
	// TODO: ordering, e.g. Morton
	x = clamp(x, 0, descriptor.width - 1);
	y = clamp(y, 0, descriptor.height - 1);
	return uint(x + y * descriptor.width) * pixelSizeInBytes;
}

Framebuffer::Framebuffer(const FramebufferDesc &desc)
{
	descriptor.width = max(desc.width, 1u);
	descriptor.height = max(desc.height, 1u);
	descriptor.format = desc.format;

	aspectRatio = Real(descriptor.width) / Real(descriptor.height);

	// Number of channels
	switch (descriptor.format)
	{
		case FramebufferFormat::r32f:
		case FramebufferFormat::r32ui:
		case FramebufferFormat::r32si:
		{
			channelAmount = 1;
			break;
		}
		case FramebufferFormat::r32g32b32f:
		case FramebufferFormat::r32g32b32ui:
		case FramebufferFormat::r32g32b32si:
		{
			channelAmount = 3;
			break;
		}
		default:
			channelAmount = 0;
	};

	// Number of bytes in one pixel
	switch (descriptor.format)
	{
		case FramebufferFormat::r32f:
		case FramebufferFormat::r32g32b32f:
		{
			pixelSizeInBytes = sizeof(float);
			break;
		}
		case FramebufferFormat::r32ui:
		case FramebufferFormat::r32g32b32ui:
		{
			pixelSizeInBytes = sizeof(uint);
			break;
		}
		case FramebufferFormat::r32si:
		case FramebufferFormat::r32g32b32si:
		{
			pixelSizeInBytes = sizeof(int);
			break;
		}
		default:
			pixelSizeInBytes = 0;
	};
	pixelSizeInBytes *= channelAmount;

	dataSize = pixelSizeInBytes * descriptor.width * descriptor.height;
	data = new byte[dataSize]();
}

void Framebuffer::operator=(const Framebuffer &other)
{
	if (descriptor != other.descriptor)
	{
		delete[] data;
		std::memcpy(this, &other, sizeof(other));
		data = new byte[dataSize];
	}

	std::memcpy(data, other.data, dataSize);
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

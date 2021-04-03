#pragma once

#include "Common.hpp"

#include "Math.hpp"

#include <cstring>	// For memcpy

enum class ImageFormat
{
	// How to read:
	// ImageFormat::AX[BY[CZ[DW]]]F
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

// Descriptor for the image construction
struct ImageDesc
{
	uint width = 1;
	uint height = 1;
	ImageFormat format = ImageFormat::r32f;

	bool operator==(const ImageDesc &other) const;
	bool operator!=(const ImageDesc &other) const { return !(*this == other); }
};

bool ImageDesc::operator==(const ImageDesc &other) const
{
	return width == other.width &&
		height == other.height &&
		format == other.format;
}

class Image
{
private:
	ImageDesc descriptor;
	byte *data = nullptr;

	// Meta data
	Real aspectRatio = 0.0;
	uint channelAmount = 0;
	uint pixelSizeInBytes = 0;
	uint dataSize = 0;

	uint positionToIndex(int x, int y) const;

public:
	Image(const ImageDesc &desc);
	~Image() { delete[] data; }

	void operator=(const Image &other);

	ImageDesc getDesc() const { return descriptor; }
	uint getWidth() const { return descriptor.width; }
	uint getHeight() const { return descriptor.height; }
	Real getAspectRatio() const { return aspectRatio; }
	uint getChannelAmount() const { return channelAmount; }
	uint getPixelSizeInBytes() const { return pixelSizeInBytes; }
	const byte *getData() const { return data; }
	void store(int x, int y, const byte *in);
	void load(int x, int y, byte *out) const;
};

uint Image::positionToIndex(int x, int y) const
{
	x = clamp(x, 0, descriptor.width - 1);
	y = clamp(y, 0, descriptor.height - 1);
	return uint(x + y * descriptor.width) * pixelSizeInBytes;
}

Image::Image(const ImageDesc &desc)
{
	descriptor.width = max(desc.width, 1u);
	descriptor.height = max(desc.height, 1u);
	descriptor.format = desc.format;

	aspectRatio = Real(descriptor.width) / Real(descriptor.height);

	// Number of channels
	switch (descriptor.format)
	{
		case ImageFormat::r32f:
		case ImageFormat::r32ui:
		case ImageFormat::r32si:
		{
			channelAmount = 1;
			break;
		}
		case ImageFormat::r32g32b32f:
		case ImageFormat::r32g32b32ui:
		case ImageFormat::r32g32b32si:
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
		case ImageFormat::r32f:
		case ImageFormat::r32g32b32f:
		{
			pixelSizeInBytes = sizeof(float);
			break;
		}
		case ImageFormat::r32ui:
		case ImageFormat::r32g32b32ui:
		{
			pixelSizeInBytes = sizeof(uint);
			break;
		}
		case ImageFormat::r32si:
		case ImageFormat::r32g32b32si:
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

void Image::operator=(const Image &other)
{
	if (descriptor != other.descriptor)
	{
		delete[] data;
		std::memcpy(this, &other, sizeof(other));
		data = new byte[dataSize];
	}

	std::memcpy(data, other.data, dataSize);
}

void Image::store(int x, int y, const byte *in)
{
	uint index = positionToIndex(x, y);
	std::memcpy(&data[index], in, pixelSizeInBytes);
}

void Image::load(int x, int y, byte *out) const
{
	uint index = positionToIndex(x, y);
	std::memcpy(out, &data[index], pixelSizeInBytes);
}

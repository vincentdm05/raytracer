#pragma once

#include "Common.hpp"

#include "Math.hpp"
#include "Vec3.hpp"
#include "Viewport.hpp"

#include <fstream>
#include <iostream>
#include <string>

template <typename DataType>
class Framebuffer
{
private:
	uint width = 0;
	uint height = 0;
	DataType *data = nullptr;

	uint positionToIndex(int x, int y) const;

public:
	Framebuffer(const Viewport &vp);
	~Framebuffer() { delete[] data; }

	void store(int x, int y, const DataType &value);
	void blend(int x, int y, const DataType &value);
	DataType load(int x, int y) const;
	DataType sample(Real u, Real v) const;
	void writePpm(const std::string &fileName) const;
};

template <typename DataType>
uint Framebuffer<DataType>::positionToIndex(int x, int y) const
{
	// TODO: ordering, e.g. Morton
	x = clamp(x, 0, width - 1);
	y = clamp(y, 0, height - 1);
	return uint(x + y * width);
}

template <typename DataType>
Framebuffer<DataType>::Framebuffer(const Viewport &vp)
{
	width = max(vp.width(), 1);
	height = max(vp.height(), 1);
	data = new DataType[width * height]();
}

template <typename DataType>
void Framebuffer<DataType>::store(int x, int y, const DataType &value)
{
	uint index = positionToIndex(x, y);
	data[index] = value;
}

template <typename DataType>
void Framebuffer<DataType>::blend(int x, int y, const DataType &value)
{
	uint index = positionToIndex(x, y);
	// TODO: blend functions
	data[index] += value;
}

template <typename DataType>
DataType Framebuffer<DataType>::load(int x, int y) const
{
	uint index = positionToIndex(x, y);
	return data[index];
}

template <typename DataType>
DataType Framebuffer<DataType>::sample(Real u, Real v) const
{
	// TODO: filtering
	int x = int(round(u * width));
	int y = int(round(v * height));
	return load(x, y);
}

template <>
void Framebuffer<Vec3>::writePpm(const std::string &baseFileName) const
{
	std::string fileName(baseFileName);
	std::string extension(".ppm");
	if (baseFileName.size() <= extension.size() ||
		baseFileName.compare(baseFileName.size() - extension.size(), extension.size(), extension) != 0)
	{
		fileName += extension;
	}
	std::ofstream file(fileName);

	if (!file.is_open())
	{
		std::cerr << "Could not open file " << fileName << " for writing." << std::endl;
		return;
	}

	// PPM header
	file << "P3\n" << width << " " << height << "\n255\n";

	for (int row = int(height) - 1; row >= 0; row--)
	{
		for (int col = 0; col < int(width); col++)
		{
			Vec3 colour = load(col, row);
			file << int(colour.r) << " " << int(colour.g) << " " << int(colour.b) << "\n";
		}
	}

	file.close();
	std::cout << "Result written to " << fileName << std::endl;
}

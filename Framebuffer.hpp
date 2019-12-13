#pragma once

#include "Common.hpp"

#include "Math.hpp"
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

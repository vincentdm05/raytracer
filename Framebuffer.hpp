#pragma once

#include "Common.hpp"

#include "Math.hpp"
#include "Vec3.hpp"
#include "Viewport.hpp"

class Framebuffer
{
private:
	uint width = 0;
	uint height = 0;
	Vec3 *data = nullptr;

	uint positionToIndex(int x, int y) const;

public:
	Framebuffer(const Viewport &vp);
	~Framebuffer() { delete[] data; }

	void store(int x, int y, const Vec3 &value);
	void blend(int x, int y, const Vec3 &value);
	Vec3 load(int x, int y) const;
	Vec3 sample(Real u, Real v) const;
	void printImage() const;
};

uint Framebuffer::positionToIndex(int x, int y) const
{
	// TODO: ordering, e.g. Morton
	x = clamp(x, 0, width - 1);
	y = clamp(y, 0, height - 1);
	return uint(x + y * width);
}

Framebuffer::Framebuffer(const Viewport &vp)
{
	width = max(vp.width(), 1);
	height = max(vp.height(), 1);
	data = new Vec3[width * height]();
}

void Framebuffer::store(int x, int y, const Vec3 &value)
{
	uint index = positionToIndex(x, y);
	data[index] = value;
}

void Framebuffer::blend(int x, int y, const Vec3 &value)
{
	uint index = positionToIndex(x, y);
	// TODO: blend functions
	data[index] += value;
}

Vec3 Framebuffer::load(int x, int y) const
{
	uint index = positionToIndex(x, y);
	return data[index];
}

Vec3 Framebuffer::sample(Real u, Real v) const
{
	// TODO: filtering
	int x = int(round(u * width));
	int y = int(round(v * height));
	return load(x, y);
}

void Framebuffer::printImage() const
{
	// PPM header
	std::cout << "P3\n" << width << " " << height << "\n255\n";

	for (int row = height - 1; row >= 0; row--)
	{
		for (int col = 0; col < width; col++)
		{
			Vec3 colour = load(col, row);
			std::cout << int(colour.r) << " " << int(colour.g) << " " << int(colour.b) << "\n";
		}
	}
}

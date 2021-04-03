#pragma once

#include "Common.hpp"

#include "Image.hpp"
#include "Vec3.hpp"

#include <fstream>
#include <iostream>
#include <string>

namespace file
{

// By default (range <= 0) the maximum value is searched in the image during traversal.
bool writePpm(const std::string& baseFileName, const Image &image, int range = -1)
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
		return false;
	}

	uint width = image.getWidth();
	uint height = image.getHeight();

	// PPM header
	file << "P3\n" << width << " " << height << "\n";
	std::streampos headerPosForRange = file.tellp();
	// 5 placeholder spaces (maximum supported by PPM is 65535)
	file << "     \n";

	int maxValue = max(1, range);
	Real colourArray[3];
	for (int row = int(height) - 1; row >= 0; row--)
	{
		for (int col = 0; col < int(width); col++)
		{
			image.load(col, row, (byte*)colourArray);
			Vec3 colour(colourArray[0], colourArray[1], colourArray[2]);
			if (range <= 0)
				maxValue = max(int(max(colour)), maxValue);
			file << int(colour.r) << " " << int(colour.g) << " " << int(colour.b) << "\n";
		}
	}
	maxValue = min(maxValue, 65535);

	file.seekp(headerPosForRange);
	file << maxValue;

	file.close();
	std::cout << "Result written to " << fileName << std::endl;

	return true;
}

}	// namespace file

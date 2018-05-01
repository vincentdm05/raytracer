#include <iostream>

int main()
{
	int nx = 200;
	int ny = 100;
	std::cout << "P3\n" << nx << " " << ny << "\n255\n";
	for (int row = ny - 1; row >= 0; row--)
	{
		for (int col = 0; col < nx; col++)
		{
			float r = float(col) / nx;
			float g = float(row) / ny;
			float b = 0.2f;
			float ir = int(r * 255.99);
			float ig = int(g * 255.99);
			float ib = int(b * 255.99);
			std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}
	return 0;
}

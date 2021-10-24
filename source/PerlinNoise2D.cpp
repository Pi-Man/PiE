
#include "PerlinNoise2D.h"

#include <random>



double PerlinNoise2D::gradientDot(int hash, double x, double y) const {

	int side = hash >> 6;

	double distance = (hash & 63) / 32.0 - 1.0;

	if (side == 0) {
		return x + y * distance;
	}
	else if (side == 1) {
		return x * distance + y;
	}
	else if (side == 2) {
		return -x + y * distance;
	}
	else if (side == 3) {
		return x * distance - y;
	}

	return 0.0;
}

double PerlinNoise2D::fade(double k) const {

	return k * k * k * (k * (k * 6 - 15) + 10);
}

PerlinNoise2D::PerlinNoise2D(long seed){

	std::default_random_engine rand(seed);

	int i;

	for (i = 0; i < 256; i++) {
		this->hashTable[i] = rand() & 255;
	}

	for (; i < 512; i++) {
		this->hashTable[i] = this->hashTable[i & 255];
	}

}

double PerlinNoise2D::get(double x, double y) const {

	int xi = (int)floor(x) & 255;
	int yi = (int)floor(y) & 255;

	double dx = x - floor(x);
	double dy = y - floor(y);
	
	int hash00 = hashTable[hashTable[xi] + yi];
	int hash01 = hashTable[hashTable[xi] + yi + 1];
	int hash10 = hashTable[hashTable[xi + 1] + yi];
	int hash11 = hashTable[hashTable[xi + 1] + yi + 1];

	double u = fade(dx);
	double v = fade(dy);

	double x1 = Utils::lerp(gradientDot(hash00, dx, dy), gradientDot(hash10, dx - 1, dy), u);
	double x2 = Utils::lerp(gradientDot(hash01, dx, dy - 1), gradientDot(hash11, dx - 1, dy - 1), u);

	return Utils::lerp(x1, x2, v) / 2.0 + 0.5;
}

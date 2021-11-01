#ifndef _PERLIN_NOISE_2D
#define _PERLIN_NOISE_2D

#include <SDL.h>

#include "Utils.h"

class PerlinNoise2D
{

	int hashTable[512];

	double gradientDot(int hash, double x, double y) const;
	double fade(double k) const;

public:
	// create a new instance of noise with the given seed, or set the seed based on the program run time by default
	PerlinNoise2D(long seed = SDL_GetTicks());

	// get value at specified location
	// * returns -1 to 1
	double get(double x, double y) const;

};

#endif
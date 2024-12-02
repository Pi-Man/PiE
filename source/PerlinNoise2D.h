#ifndef _PERLIN_NOISE_2D_H
#define _PERLIN_NOISE_2D_H

#include <SDL.h>

#include "Utils.h"

namespace PiE {

	struct PerlinNoise2D {

		int hashTable[512];

		double gradientDot(int hash, double x, double y) const;
		double fade(double k) const;

		// create a new instance of noise with the given seed, or set the seed based on the program run time by default
		PerlinNoise2D(long seed = SDL_GetTicks());

		// get value at specified location
		// * returns -1 to 1
		double operator()(double x, double y) const;

		bool operator==(const PerlinNoise2D & other) const;
	};

}

#endif
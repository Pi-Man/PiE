
#include<random>

#include<SDL.h>

#include "FourierNoiseGenerator.h"

void FourierNoiseGenerator::genCoeficients(int numCoeficients, int dimensions, long seed) {
	coeficients = new float[dimensions * numCoeficients * 2];
	std::default_random_engine rand(seed);

	for (int i = 0; i < numCoeficients * dimensions * 2; i++) {
		coeficients[i] = (float)rand() / (float)rand.max();
	}
}

FourierNoiseGenerator::FourierNoiseGenerator(int numCoeficients, int dimensions) : numCoeficients(numCoeficients), dimensions(dimensions) {
	genCoeficients(numCoeficients, dimensions, SDL_GetTicks());
}

FourierNoiseGenerator::FourierNoiseGenerator(int numCoeficients, int dimensions, long seed) : numCoeficients(numCoeficients), dimensions(dimensions) {
	genCoeficients(numCoeficients, dimensions, seed);
}

FourierNoiseGenerator::~FourierNoiseGenerator() {
	delete coeficients;
}

double FourierNoiseGenerator::get(std::vector<float> vector) {

	if (vector.size() != dimensions) throw new std::runtime_error("incorrect vector size");

	double result = 0;
	double divisor = 0;

	for (int j = 0; j < dimensions; j++){
		for (int i = 0; i < numCoeficients; i++) {
			result += (coeficients[i * 2 + j * numCoeficients * 2] * sin(vector[j] * pow(2, i) * 3.14*2) + coeficients[i * 2 + 1 + j * numCoeficients * 2] * cos(vector[j] * pow(2, i) * 3.14*2)) / pow(2, i);
			//divisor += coeficients[i + j * numCoeficients];
		}
	}

	return result/10 + 0.5;
}

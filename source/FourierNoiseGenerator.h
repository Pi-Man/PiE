#ifndef _FOURIER_NOISE_GENERATOR
#define _FOURIER_NOISE_GENERATOR

class FourierNoiseGenerator
{

	float* coeficients;
	int numCoeficients, dimensions;

	void genCoeficients(int numCoeficients, int dimensions, long seed);

public:
	FourierNoiseGenerator(int numCoeficients, int dimensions, long seed);
	FourierNoiseGenerator(int numCoeficients, int dimensions);

	~FourierNoiseGenerator();

	double get(std::vector<float> pos);
};

#endif
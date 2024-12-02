#ifndef _FOURIER_NOISE_GENERATOR_H
#define _FOURIER_NOISE_GENERATOR_H

namespace PiE {

    template<size_t N, size_t C = 10>
    struct FourierNoiseGenerator {

        const std::array<std::array<std::array<double, 2>, C>, N> coeficients;

        FourierNoiseGenerator(size_t seed = 0);

        template<typename T>
        double operator()(const std::array<T, N> & x) const;
        bool operator==(const FourierNoiseGenerator & other) const;
        bool operator!=(const FourierNoiseGenerator & other) const;
    };

	template<size_t N, size_t C>
	FourierNoiseGenerator<N, C>::FourierNoiseGenerator(size_t seed) {
		std::mt19937_64 r(seed);
		std::uniform_real_distribution<double> gen(0, 1);
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < C; j++) {
				coeficients[i][j][0] = gen(r);
				coeficients[i][j][1] = gen(r);
			}
		}
	}

	template<size_t N, size_t C>
	template<typename T>
	double ::PiE::FourierNoiseGenerator<N, C>::operator()(const std::array<T, N> & x) const {

		double result = 0;

		for (int i = 0; i < N; i++) {
			for (int j = 0; j < C; j++) {
				result += coeficients[i][j][0] * sin(x[i] * j) + coeficients[i][j][1] * cos(x[i] * j);
			}
		}

		return result/N;
	}

	template<size_t N, size_t C>
	bool FourierNoiseGenerator<N, C>::operator==(const FourierNoiseGenerator & other) const {
		return coeficients == other.coeficients;
	}

	template<size_t N, size_t C>
	bool FourierNoiseGenerator<N, C>::operator!=(const FourierNoiseGenerator & other) const {
		return coeficients != other.coeficients;
	}

}

#endif
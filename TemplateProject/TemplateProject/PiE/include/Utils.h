#ifndef _UTILS
#define _UTILS

#include "Vec.h"

namespace Utils {

	// move a vector twords another vector a specified distance
	// * from: the starting vector
	// * to: the target vector
	// * distance: maximum distance that can be traveled
	// * intended to be used to move a vector from one position to another over several frames at a specified speed
	template<typename T, int N>
	Vec<T, N> moveTowards(Vec<T, N> from, Vec<T, N> to, T distance);

	// lerp between two values
	double lerp(double a, double b, double k);
	// lerp between two values
	float lerp(float a, float b, float k);

	template<int A, int B>
	struct get_min {
		enum {
			value = std::min(A, B)
		};
	};

	template<typename T1>
	constexpr T1 abs(T1 a) {
		return a < 0 ? -a : a;
	}

}



#endif
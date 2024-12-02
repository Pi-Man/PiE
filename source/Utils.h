#ifndef _UTILS_H
#define _UTILS_H

#include "Vec.h"

namespace PiE {

	// move a vector twords another vector a specified distance
	// * from: the starting vector
	// * to: the target vector
	// * distance: maximum distance that can be traveled
	// * intended to be used to move a vector from one position to another over several frames at a specified speed
	template<typename T, int N>
	Vec<T, N> moveTowards(Vec<T, N> from, Vec<T, N> to, T distance);

	// linear interpolation, not clamped
	template<typename T, typename _ = std::enable_if_t<
		std::is_same_v<decltype((T)(std::declval<T>() * std::declval<T>())), T> &&
		std::is_same_v<decltype((T)(std::declval<T>() + std::declval<T>())), T> &&
		std::is_same_v<decltype((T)(std::declval<T>() - std::declval<T>())), T>
	>>
	T lerp(T a, T b, T k) {
		return a + k * (b - a);
	}

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
#include "Utils.h"

namespace PiE {

	template<typename T, int N>
	Vec<T, N> moveTowards(Vec<T, N> from, Vec<T, N> to, T distance) {
		Vec3f d = to - from;
		T mag = d();
		if (mag < distance) return to;
		return from + d / mag * distance;
	}

	double lerp(double a, double b, double k) {
		return a + k * (b - a);
	}

	float lerp(float a, float b, float k) {
		return a + k * (b - a);
	}

}
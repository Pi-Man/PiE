#include "Utils.h"

Vec3f Utils::moveTowards(Vec3f from, Vec3f to, float distance) {
	Vec3f d = to - from;
	if (d() < distance) return to;
	return from + d / d() * distance;
}

double Utils::lerp(double a, double b, double k) {
	return a + k * (b - a);
}

float Utils::lerp(float a, float b, float k) {
	return a + k * (b - a);
}
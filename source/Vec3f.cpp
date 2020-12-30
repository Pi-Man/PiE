#include "Vec3f.h"

Vec3f::Vec3f() : data({ 0, 0, 0 }) {}

Vec3f::Vec3f(std::array<float, 3> in) : data(in) {}

Vec3f Vec3f::operator+(const Vec3f other) const {
	return Vec3f({ data[0] + other[0], data[1] + other[1], data[2] + other[2] });
}

Vec3f & Vec3f::operator+=(Vec3f other) {
	data[0] += other[0];
	data[1] += other[1];
	data[2] += other[2];
	return *this;
}

Vec3f Vec3f::operator-(const Vec3f other) const {
	return Vec3f({ data[0] - other[0], data[1] - other[1], data[2] - other[2] });
}

Vec3f & Vec3f::operator-=(const Vec3f other) {
	data[0] -= other[0];
	data[1] -= other[1];
	data[2] -= other[2];
	return *this;
}

Vec3f Vec3f::operator-() const {
	return Vec3f({ -data[0], -data[1], -data[2] });
}

float Vec3f::operator*(const Vec3f other) const {
	return data[0] * other[0] + data[1] * other[1] + data[2] * other[2];
}

Vec3f Vec3f::operator*(const float scale) const {
	return Vec3f({ data[0] * scale, data[1] * scale, data[2] * scale });
}

Vec3f & Vec3f::operator*=(const float scale) {
	data[0] *= scale;
	data[1] *= scale;
	data[2] *= scale;
	return *this;
}

Vec3f Vec3f::operator/(const float scale) const {
	return Vec3f({ data[0] / scale, data[1] / scale, data[2] / scale });
}

Vec3f & Vec3f::operator/=(const float scale) {
	data[0] /= scale;
	data[1] /= scale;
	data[2] /= scale;
	return *this;
}

Vec3f Vec3f::operator^(const Vec3f other) const {
	Vec3f temp;
	temp[0] = data[1] * other[2] - data[2] * other[1];
	temp[1] = data[2] * other[0] - data[0] * other[2];
	temp[2] = data[0] * other[1] - data[1] * other[0];
	return temp;
}

Vec3f & Vec3f::operator^=(const Vec3f other) {
	Vec3f temp(*this);
	data[0] = temp[1] * other[2] - temp[2] * other[1];
	data[1] = temp[2] * other[0] - temp[0] * other[2];
	data[2] = temp[0] * other[1] - temp[1] * other[0];
	return *this;
}

float &Vec3f::operator[](const int i) {
	return data[i];
}

const float &Vec3f::operator[](const int i) const {
	return data[i];
}

float Vec3f::operator()(int pwr) {
	if (pwr == 1) return sqrtf(data[0]*data[0] + data[1]*data[1] + data[2]*data[2]);
	if (pwr == 2) return data[0] * data[0] + data[1] * data[1] + data[2] * data[2];
	return -1;
}

Vec3f Vec3f::lerp(Vec3f a, Vec3f b, float k) {
	return Vec3f({a[0] + k * (b[0] - a[0]), a[1] + k * (b[1] - a[1]) , a[2] + k * (b[2] - a[2]) });
}

Vec3f::operator std::array<float, 3>() const {
	return data;
}

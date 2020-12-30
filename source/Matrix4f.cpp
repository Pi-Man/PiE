#define _USE_MATH_DEFINES

#include <cmath>

#include "Matrix4f.h"

Matrix4f::Matrix4f() : 
	m
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	}
	{}

Matrix4f::Matrix4f(const std::array<float, 16>& input) : m(input) {}

Matrix4f::~Matrix4f() {}

Matrix4f Matrix4f::identity{};

float& Matrix4f::operator[] (int index) {
	return m[index];
}

const float &Matrix4f::operator[] (int index) const {
	return m[index];
}

Matrix4f Matrix4f::operator* (const Matrix4f& other) const {
	Matrix4f mTemp;
	mTemp[0]  = m[0]  * other[0] + m[1]  * other[4] + m[2]  * other[8]  + m[3]  * other[12];
	mTemp[1]  = m[0]  * other[1] + m[1]  * other[5] + m[2]  * other[9]  + m[3]  * other[13];
	mTemp[2]  = m[0]  * other[2] + m[1]  * other[6] + m[2]  * other[10] + m[3]  * other[14];
	mTemp[3]  = m[0]  * other[3] + m[1]  * other[7] + m[2]  * other[11] + m[3]  * other[15];
	mTemp[4]  = m[4]  * other[0] + m[5]  * other[4] + m[6]  * other[8]  + m[7]  * other[12];
	mTemp[5]  = m[4]  * other[1] + m[5]  * other[5] + m[6]  * other[9]  + m[7]  * other[13];
	mTemp[6]  = m[4]  * other[2] + m[5]  * other[6] + m[6]  * other[10] + m[7]  * other[14];
	mTemp[7]  = m[4]  * other[3] + m[5]  * other[7] + m[6]  * other[11] + m[7]  * other[15];
	mTemp[8]  = m[8]  * other[0] + m[9]  * other[4] + m[10] * other[8]  + m[11] * other[12];
	mTemp[9]  = m[8]  * other[1] + m[9]  * other[5] + m[10] * other[9]  + m[11] * other[13];
	mTemp[10] = m[8]  * other[2] + m[9]  * other[6] + m[10] * other[10] + m[11] * other[14];
	mTemp[11] = m[8]  * other[3] + m[9]  * other[7] + m[10] * other[11] + m[11] * other[15];
	mTemp[12] = m[12] * other[0] + m[13] * other[4] + m[14] * other[8]  + m[15] * other[12];
	mTemp[13] = m[12] * other[1] + m[13] * other[5] + m[14] * other[9]  + m[15] * other[13];
	mTemp[14] = m[12] * other[2] + m[13] * other[6] + m[14] * other[10] + m[15] * other[14];
	mTemp[15] = m[12] * other[3] + m[13] * other[7] + m[14] * other[11] + m[15] * other[15];	
	return mTemp;
}

Matrix4f& Matrix4f::operator*= (const Matrix4f& other) {
	Matrix4f mTemp;
	mTemp[0]  = m[0]  * other[0] + m[1]  * other[4] + m[2]  * other[8]  + m[3]  * other[12];
	mTemp[1]  = m[0]  * other[1] + m[1]  * other[5] + m[2]  * other[9]  + m[3]  * other[13];
	mTemp[2]  = m[0]  * other[2] + m[1]  * other[6] + m[2]  * other[10] + m[3]  * other[14];
	mTemp[3]  = m[0]  * other[3] + m[1]  * other[7] + m[2]  * other[11] + m[3]  * other[15];
	mTemp[4]  = m[4]  * other[0] + m[5]  * other[4] + m[6]  * other[8]  + m[7]  * other[12];
	mTemp[5]  = m[4]  * other[1] + m[5]  * other[5] + m[6]  * other[9]  + m[7]  * other[13];
	mTemp[6]  = m[4]  * other[2] + m[5]  * other[6] + m[6]  * other[10] + m[7]  * other[14];
	mTemp[7]  = m[4]  * other[3] + m[5]  * other[7] + m[6]  * other[11] + m[7]  * other[15];
	mTemp[8]  = m[8]  * other[0] + m[9]  * other[4] + m[10] * other[8]  + m[11] * other[12];
	mTemp[9]  = m[8]  * other[1] + m[9]  * other[5] + m[10] * other[9]  + m[11] * other[13];
	mTemp[10] = m[8]  * other[2] + m[9]  * other[6] + m[10] * other[10] + m[11] * other[14];
	mTemp[11] = m[8]  * other[3] + m[9]  * other[7] + m[10] * other[11] + m[11] * other[15];
	mTemp[12] = m[12] * other[0] + m[13] * other[4] + m[14] * other[8]  + m[15] * other[12];
	mTemp[13] = m[12] * other[1] + m[13] * other[5] + m[14] * other[9]  + m[15] * other[13];
	mTemp[14] = m[12] * other[2] + m[13] * other[6] + m[14] * other[10] + m[15] * other[14];
	mTemp[15] = m[12] * other[3] + m[13] * other[7] + m[14] * other[11] + m[15] * other[15];
	*this = mTemp;
	return *this;
}

Matrix4f Matrix4f::operator* (const float& other) const {
	Matrix4f mTemp;

	for (int i = 0; i < 16; i++) {
		mTemp[i] = m[i] * other;
	}

	return mTemp;
}

Matrix4f &Matrix4f::operator*= (const float& other) {
	Matrix4f mTemp;

	for (int i = 0; i < 16; i++) {
		m[i] *= other;
	}

	return *this;
}

Vec3f Matrix4f::operator*(const Vec3f & other) const {
	Vec3f temp;
	temp[0] = m[0] * other[0] + m[1] * other[1] + m[2] * other[2] + m[3];
	temp[1] = m[4] * other[0] + m[5] * other[1] + m[6] * other[2] + m[7];
	temp[2] = m[8] * other[0] + m[9] * other[1] + m[10] * other[2] + m[11];
	return temp;
}

Matrix4f & Matrix4f::translateX(const float distance) {
	m[3] += distance;
	return *this;
}

Matrix4f & Matrix4f::translateY(const float distance) {
	m[7] += distance;
	return *this;
}

Matrix4f & Matrix4f::translateZ(const float distance) {
	m[11] += distance;
	return *this;
}

Matrix4f & Matrix4f::translate(const float distanceX, const float distanceY, const float distanceZ) {
	m[3] += distanceX;
	m[7] += distanceY;
	m[11] += distanceZ;
	return *this;
}

Matrix4f & Matrix4f::translate(const Vec3f distance) {
	m[3] += distance[0];
	m[7] += distance[1];
	m[11] += distance[2];
	return *this;
}

Matrix4f& Matrix4f::rotateXd(const float degrees) {
	return rotateXr(degrees * M_PI / 180.0f);
}

Matrix4f& Matrix4f::rotateXr(const float radians) {
	Matrix4f rotation (
		{
			1, 0, 0, 0,
			0, cos(radians), sin(radians), 0,
			0, -sin(radians), cos(radians), 0,
			0, 0, 0, 1
		});
	*this = rotation * *this;
	return *this;
}

Matrix4f& Matrix4f::rotateYd(const float degrees) {
	return rotateYr(degrees * M_PI / 180.0f);
}

Matrix4f& Matrix4f::rotateYr(const float radians) {
	Matrix4f rotation(
		{
			cos(radians), 0, sin(radians), 0,
			0, 1, 0, 0,
			-sin(radians), 0, cos(radians), 0,
			0, 0, 0, 1
		});
	*this = rotation * *this;
	return *this;
}

Matrix4f& Matrix4f::rotateZd(const float degrees) {
	return rotateZr(degrees * M_PI / 180.0f);
}

Matrix4f& Matrix4f::rotateZr(const float radians) {
	Matrix4f rotation(
		{
			cos(radians), sin(radians), 0, 0,
			-sin(radians), cos(radians), 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		});
	*this = rotation * *this;
	return *this;
}

Matrix4f & Matrix4f::scaleX(const float scale) {
	Matrix4f temp;
	temp[0] *= scale;
	*this = temp * *this;
	return *this;
}

Matrix4f & Matrix4f::scaleY(const float scale) {
	Matrix4f temp;
	temp[5] *= scale;
	*this = temp * *this;
	return *this;
}

Matrix4f & Matrix4f::scaleZ(const float scale) {
	Matrix4f temp;
	temp[10] *= scale;
	*this = temp * *this;
	return *this;
}

Matrix4f & Matrix4f::scale(const float scale) {
	Matrix4f temp;
	temp[0] *= scale;
	temp[5] *= scale;
	temp[10] *= scale;
	*this = temp * *this;
	return *this;
}

Matrix4f & Matrix4f::scale(const float scaleX, const float scaleY, const float scaleZ) {
	Matrix4f temp;
	temp[0] *= scaleX;
	temp[5] *= scaleY;
	temp[10] *= scaleZ;
	*this = temp * *this;
	return *this;
}

Matrix4f & Matrix4f::scale(const Vec3f scale) {
	Matrix4f temp;
	temp[0] *= scale[0];
	temp[5] *= scale[1];
	temp[10] *= scale[2];
	*this = temp * *this;
	return *this;
}

Matrix4f& Matrix4f::transpose() {
	Matrix4f mTemp;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			mTemp[i + 4 * j] = m[j + 4 * i];
		}
	}

	*this = mTemp;

	return *this;
}

Matrix4f Matrix4f::transpose(const Matrix4f& m) {
	Matrix4f mTemp;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			mTemp[i + 4 * j] = m[j + 4 * i];
		}
	}

	return mTemp;
}

Matrix4f & Matrix4f::invert() {
	
	Matrix4f inverse;

	for (int i = 0; i < 4; i++) {
		int j = i - 1;
		float scale = 0;
		while (j < 3 && !scale) {
			scale = m[++j * 4 + i];
		}
		if (!scale) throw std::runtime_error("Degenerate Matrix");
		for (int i2 = 0; i2 < 4; i2++) {
			m[j * 4 + i2] /= scale;
			inverse[j * 4 + i2] /= scale;
		}
		for (int j2 = 0; j2 < 4; j2++) {
			if (j2 == j) continue;
			scale = m[j2 * 4 + i];
			for (int i2 = 0; scale && i2 < 4; i2++) {
				m[j2 * 4 + i2] -= m[j * 4 + i2] * scale;
				inverse[j2 * 4 + i2] -= inverse[j * 4 + i2] * scale;
			}
		}

	}

	*this = inverse;

	return *this;
}

Matrix4f Matrix4f::invert() const {

	Matrix4f inverse;
	Matrix4f mtemp(m);

	for (int i = 0; i < 4; i++) {
		int j;
		double scale;
		for (j = i; (scale = mtemp[i + j * 4]) == 0 && j < 4; j++);
		if (scale == 0) continue;
		for (int i2 = i; i2 < 4; i2++) {
			mtemp[i2 + j * 4] /= scale;
			inverse[i2 + j * 4] /= scale;
		}
		for (int k = 0; k < 4; k++) {
			if (k == j) continue;
			double scale2 = mtemp[i + k * 4];
			if (scale2 != 0) {
				for (int i2 = i; i2 < 4; i2++) {
					mtemp[i2 + k * 4] -= mtemp[i2 + j * 4] * scale2;
					inverse[i2 + k * 4] -= inverse[i2 + j * 4] * scale2;
				}
			}
		}
	}

	return inverse;
}

Matrix4f Matrix4f::invert(const Matrix4f & m) {

	Matrix4f inverse;
	Matrix4f mtemp = m;

	for (int i = 0; i < 4; i++) {
		int j;
		double scale;
		for (j = i; (scale = mtemp[i + j * 4]) == 0 && j < 4; j++);
		if (scale == 0) continue;
		for (int i2 = i; i2 < 4; i2++) {
			mtemp[i2 + j * 4] /= scale;
			inverse[i2 + j * 4] /= scale;
		}
		for (int k = 0; k < 4; k++) {
			if (k == j) continue;
			double scale2 = mtemp[i + k * 4];
			if (scale2 != 0) {
				for (int i2 = i; i2 < 4; i2++) {
					mtemp[i2 + k * 4] -= mtemp[i2 + j * 4] * scale2;
					inverse[i2 + k * 4] -= inverse[i2 + j * 4] * scale2;
				}
			}
		}
	}

	return inverse;
}

Matrix4f Matrix4f::lerp(const Matrix4f & left, const Matrix4f & right, const float k) {

	Matrix4f mTemp;

	for (int i = 0; i < 16; i++) {
		mTemp[i] = left[i] + (right[i] - left[i]) * k;
	}

	return mTemp;
}

Matrix4f::operator const float* () {
	return this->m.data();
}


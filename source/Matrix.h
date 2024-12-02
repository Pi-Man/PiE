#ifndef _MATRIX_H
#define _MATRIX_H

#ifdef min
#undef min
#endif // min

#ifdef max
#undef max
#endif // max

#include<array>
#include<stdexcept>

#include "Vec.h"
#include "Utils.h"

namespace PiE {

	enum AXIS {
		X,
		Y,
		Z,
		W
	};

	// generic matrix class with some extra functions for use for transformations
	template<typename T, int H, int W = H>
	struct Matrix {

		std::array<T, W * H> m = { 0 };

		// creates identity matrix by default
		Matrix();
		// creates matrix with values from supplied array (row major)
		Matrix(const std::array<T, W * H> & values);
		// creates matrix with values from supplied array (row major)
		Matrix(std::array<T, W * H> && values);

		constexpr static Matrix Identity();

		// get value by single index left to right top to botom (0-15)
		constexpr T & operator[] (const int index);
		// get value by single index left to right top to botom (0-15)
		constexpr T operator[] (const int index) const;

		// matrix multiplication: this * other
		template<int W2>
		constexpr Matrix<T, H, W2> operator* (const Matrix<T, W, W2> & other) const;
		// matrix multiplication: this = this * other
		template<int W2>
		constexpr Matrix<T, H, W2> & operator*= (const Matrix<T, W, W2> & other);

		// scalar multiplication
		constexpr Matrix operator* (const T other) const;
		// scalar multiplication
		constexpr Matrix & operator*= (const T other);

		// vector multiplication this * other
		template<typename _ = std::enable_if_t<W == H>>
		constexpr Vec<T, W-1> operator* (const Vec<T, W-1> & other) const;
		// vector multiplication this * other
		constexpr Vec<T, W> operator* (const Vec<T, W> & other) const;

		// translate along the specified axis
		template<int AXIS, typename _ = std::enable_if_t<AXIS < H>>
		constexpr Matrix & translate(const T distance);
		// add a translation along a vector
		constexpr Matrix & translate(const Vec<T, H-1> & distance);

		// translate along the specified axis
		template<int AXIS, typename _ = std::enable_if_t<AXIS < H>>
		constexpr Matrix translate(const T distance) const;
		// add a translation along a vector
		constexpr Matrix translate(const Vec<T, H - 1> & distance) const;


		// rotate by degrees around the specified plane
		template<int AXIS_1, int AXIS_2, typename _ = std::enable_if<AXIS_1 < W && AXIS_1 < H && AXIS_2 < W && AXIS_2 < H>>
		constexpr Matrix & rotateD(const T degrees);
		// rotate by radians around the specified plane
		template<int AXIS_1, int AXIS_2, typename _ = std::enable_if<AXIS_1 < W && AXIS_1 < H && AXIS_2 < W && AXIS_2 < H>>
		constexpr Matrix & rotateR(const T radians);

		// rotate by degrees around the specified plane
		template<int AXIS_1, int AXIS_2, typename _ = std::enable_if<AXIS_1 < W && AXIS_1 < H && AXIS_2 < W && AXIS_2 < H>>
		constexpr Matrix rotateD(const T degrees) const;
		// rotate by radians around the specified plane
		template<int AXIS_1, int AXIS_2, typename _ = std::enable_if<AXIS_1 < W && AXIS_1 < H && AXIS_2 < W && AXIS_2 < H>>
		constexpr Matrix rotateR(const T radians) const;

		// scale along the specified axis, -1 for all axis
		template<int AXIS, typename _ = std::enable_if_t<AXIS < W>>
		constexpr Matrix & scale(const T scale);
		// scale along a vector
		constexpr Matrix & scale(const Vec<T, get_min<H, W>::value> scale);

		// transpose the matrix
		constexpr Matrix<T, W, H> & transpose();
		// get a transpose of the matrix (copies)
		constexpr Matrix<T, W, H> transpose() const;
		// get a transpose of the matrix (copies)
		constexpr static Matrix<T, W, H> transpose(const Matrix & m);

		// invert the matrix
		template<typename _ = std::enable_if_t<W == H>>
		constexpr Matrix & invert();
		// get an inverse of the matrix (copies)
		template<typename _ = std::enable_if_t<W == H>>
		constexpr Matrix invert() const;
		// get an inverse of the matrix (copies)
		template<typename _ = std::enable_if_t<W == H>>
		constexpr static Matrix invert(const Matrix & m);

		// direcly lerp between two matrices
		// * does not preserve scale when lerping between rotations!
		constexpr static Matrix lerp(const Matrix & left, const Matrix & right, const float k);

		// spherical lerp between two matrices
		// * normalizes the output matrix to have a lerped determinant (preserves scale compared to lerp which doesn't preserve scale)
		constexpr static Matrix slerp(const Matrix & left, const Matrix & right, const float k);

		// cast to backing array
		constexpr operator T * ();
		// read only cast to backing array
		constexpr operator const T * () const;

		// calculate determinant
		template<typename _ = std::enable_if_t<W == H>>
		constexpr T operator()() const;

		// get element at x, y
		constexpr T & operator()(int x, int y);
		// get element at x, y
		constexpr const T operator()(int x, int y) const;

		bool operator==(const Matrix & other) const;
	};

	#ifndef M_PI
	#define M_PI 3.141592653589793238462643383
	#define M_PI_DEF
	#endif

	template<typename T, int H, int W>
	Matrix<T, H, W>::Matrix() {}

	template<typename T, int H, int W>
	Matrix<T, H, W>::Matrix(const std::array<T, W * H> & input) : m(input) {}

	template<typename T, int H, int W>
	Matrix<T, H, W>::Matrix(std::array<T, W * H> && input) : m(input) {}

	template<typename T, int H, int W>
	constexpr Matrix<T, H, W> Matrix<T, H, W>::Identity() {
		Matrix<T, H, W> m;
		for (int j = 0; j < H; j++) {
			for (int i = 0; i < W; i++) {
				m(i, j) = (T)(i == j);
			}
		}
		return m;
	}

	template<typename T, int H, int W>
	constexpr T & Matrix<T, H, W>::operator[] (int index) {
		return m[index];
	}

	template<typename T, int H, int W>
	constexpr T Matrix<T, H, W>::operator[] (int index) const {
		return m[index];
	}

	template<typename T, int H, int W>
	template<int W2>
	constexpr Matrix<T, H, W2> Matrix<T, H, W>::operator* (const Matrix<T, W, W2> & other) const {
		Matrix<T, H, W2> mTemp;
		for (int i = 0; i < W2; i++) {
			for (int j = 0; j < H; j++) {
				int index = i + W2 * j;
				mTemp[index] = 0;
				for (int k = 0; k < W; k++) {
					mTemp[index] += m[k + j * W] * other[i + k * W2];
				}
			}
		}
		return mTemp;
	}

	template<typename T, int H, int W>
	template<int W2>
	constexpr Matrix<T, H, W2> & Matrix<T, H, W>::operator*= (const Matrix<T, W, W2> & other) {
		Matrix<T, H, W2> mTemp;
		for (int i = 0; i < W2; i++) {
			for (int j = 0; j < H; j++) {
				int index = i + W2 * j;
				mTemp[index] = 0;
				for (int k = 0; k < W; k++) {
					mTemp[index] += m[k + j * W] * other[i + k * W2];
				}
			}
		}
		*this = mTemp;
		return *this;
	}

	template<typename T, int H, int W>
	constexpr Matrix<T, H, W> Matrix<T, H, W>::operator* (const T other) const {
		Matrix<T, H, W> mTemp;

		for (int i = 0; i < W*H; i++) {
			mTemp[i] = m[i] * other;
		}

		return mTemp;
	}

	template<typename T, int H, int W>
	constexpr Matrix<T, H, W> & Matrix<T, H, W>::operator*= (const T other) {
		Matrix<T, H, W> mTemp;

		for (int i = 0; i < W*H; i++) {
			m[i] *= other;
		}

		return *this;
	}

	template<typename T, int H, int W>
	template<typename _>
	constexpr Vec<T, W-1> Matrix<T, H, W>::operator*(const Vec<T, W-1> & v) const {
		Vec<T, W-1> temp;
		for (int j = 0; j < H-1; j++) {
			temp[j] = 0;
			int k;
			for (k = 0; k < W-1; k++) {
				temp[j] += m[k + j * W] * v[k];
			}
			temp[j] += m[k + j * W];
		}
		return temp;
	}


	// vector multiplication other * this
	template<typename T, int H, int W, typename _ = std::enable_if_t<W == H>>
	constexpr Vec<T, H-1> operator*(const Vec<T, H-1> & v, const Matrix<T, H, W> & m) {
		Vec<T, H-1> temp;
		for (int i = 0; i < W; i++) {
			temp[i] = 0;
			int j;
			for (j = 0; j < H-1; j++) {
				temp[i] += m[i + W * j] * v[j];
			}
			temp[i] += m[i + W * j];
		}
		return temp;
	}


	// vector multiplication other * this
	template<typename T, int H, int W>
	constexpr Vec<T, W> Matrix<T, H, W>::operator*(const Vec<T, W> & v) const {
		Vec<T, W> temp;
		for (int j = 0; j < H; j++) {
			size_t index = j;
			temp[index] = 0;
			int k;
			for (k = 0; k < W; k++) {
				temp[index] += m[k + j * W] * v[k];
			}
		}
		return temp;
	}

	template<typename T, int H, int W>
	constexpr Vec<T, H> operator*(const Vec<T, H> & v, const Matrix<T, H, W> & m) {
		Vec<T, H> temp;
		for (int i = 0; i < W; i++) {
			temp[i] = 0;
			int j;
			for (j = 0; j < H; j++) {
				temp[i] += m[i + W * j] * v[j];
			}
		}
		return temp;
	}

	template<typename T, int H, int W>
	template<int AXIS, typename _>
	constexpr Matrix<T, H, W> & Matrix<T, H, W>::translate(const T distance) {
		m[W-1 + W*AXIS] += distance;
		return *this;
	}

	template<typename T, int H, int W>
	constexpr Matrix<T, H, W> & Matrix<T, H, W>::translate(const Vec<T, H-1> & distance) {
		for (int i = 0; i < H - 1; i++) {
			m[W-1 + W*i] += distance[i];
		}
		return *this;
	}

	template<typename T, int H, int W>
	template<int AXIS, typename _>
	constexpr Matrix<T, H, W> Matrix<T, H, W>::translate(const T distance) const {
		Matrix<T, H, W> matrix(m);
		matrix[W - 1 + W * AXIS] += distance;
		return matrix;
	}

	template<typename T, int H, int W>
	constexpr Matrix<T, H, W> Matrix<T, H, W>::translate(const Vec<T, H - 1> & distance) const {
		Matrix<T, H, W> matrix(m);
		for (int i = 0; i < H - 1; i++) {
			matrix[W - 1 + W * i] += distance[i];
		}
		return matrix;
	}

	template<typename T, int H, int W>
	template<int AXIS_1, int AXIS_2, typename _>
	constexpr Matrix<T, H, W> & Matrix<T, H, W>::rotateD(const T degrees) {
		return rotateR<AXIS_1, AXIS_2>(degrees * (T)M_PI / (T)180.0);
	}

	template<typename T, int H, int W>
	template<int AXIS_1, int AXIS_2, typename _>
	constexpr Matrix<T, H, W> & Matrix<T, H, W>::rotateR(const T radians) {
		Matrix<T, H, W> rotation = Matrix<T, H, W>::Identity();
		T sinr = sin(radians);
		T cosr = cos(radians);
		rotation[AXIS_1 + W * AXIS_1] = cosr;
		rotation[AXIS_2 + W * AXIS_1] = -sinr;
		rotation[AXIS_1 + W * AXIS_2] = sinr;
		rotation[AXIS_2 + W * AXIS_2] = cosr;
		*this = rotation * *this;
		return *this;
	}

	template<typename T, int H, int W>
	template<int AXIS_1, int AXIS_2, typename _>
	constexpr Matrix<T, H, W> Matrix<T, H, W>::rotateD(const T degrees) const {
		return rotateR<AXIS_1, AXIS_2>(degrees * (T)M_PI / (T)180.0);
	}

	template<typename T, int H, int W>
	template<int AXIS_1, int AXIS_2, typename _>
	constexpr Matrix<T, H, W> Matrix<T, H, W>::rotateR(const T radians) const {
		Matrix<T, H, W> rotation = Matrix<T, H, W>::Identity();
		T sinr = sin(radians);
		T cosr = cos(radians);
		rotation[AXIS_1 + W * AXIS_1] = cosr;
		rotation[AXIS_2 + W * AXIS_1] = -sinr;
		rotation[AXIS_1 + W * AXIS_2] = sinr;
		rotation[AXIS_2 + W * AXIS_2] = cosr;
		return rotation * *this;
	}

	template<typename T, int H, int W>
	template<int AXIS, typename _>
	constexpr Matrix<T, H, W> & Matrix<T, H, W>::scale(const T scale) {
		if (AXIS == -1) {
			Matrix<T, H, W> temp = Matrix<T, H, W>::Identity();
			for (int i = 0; i < std::min(H, W); i++) {
				temp[i + i * W] = scale;
			}
			*this = temp * *this;
			return *this;
		}
		Matrix<T, H, W> temp = Matrix<T, H, W>::Identity();
		temp[AXIS + W * AXIS] = scale;
		*this = temp * *this;
		return *this;
	}

	template<typename T, int H, int W>
	constexpr Matrix<T, H, W> & Matrix<T, H, W>::scale(const Vec<T, get_min<H, W>::value> scale) {
		Matrix<T, H, W> temp = Matrix<T, H, W>::Identity();
		for (int i = 0; i < std::min(H, W); i++) {
			temp[i + W * i] = scale[i];
		}
		*this = temp * *this;
		return *this;
	}

	template<typename T, int H, int W>
	constexpr Matrix<T, W, H> & Matrix<T, H, W>::transpose() {
		Matrix<T, W, H> mTemp;

		for (int i = 0; i < H; i++) {
			for (int j = 0; j < W; j++) {
				mTemp[i + H * j] = m[j + W * i];
			}
		}

		*this = mTemp;

		return *this;
	}

	template<typename T, int H, int W>
	constexpr Matrix<T, W, H> Matrix<T, H, W>::transpose() const {
		Matrix<T, W, H> mTemp;

		for (int i = 0; i < H; i++) {
			for (int j = 0; j < W; j++) {
				mTemp[i + H * j] = m[j + W * i];
			}
		}

		return mTemp;
	}

	template<typename T, int H, int W>
	constexpr Matrix<T, W, H> Matrix<T, H, W>::transpose(const Matrix<T, H, W> & m) {
		Matrix<T, W, H> mTemp;

		for (int i = 0; i < H; i++) {
			for (int j = 0; j < W; j++) {
				mTemp[i + H * j] = m[j + W * i];
			}
		}

		return mTemp;
	}

	template<typename T, int H, int W>
	template<typename _>
	constexpr Matrix<T, H, W> & Matrix<T, H, W>::invert() {

		Matrix<T, H, W> inverse = Matrix<T, H, W>::Identity();

		for (int i = 0; i < W; i++) {
			int j;
			T scale;
			for (j = i; j < W && (scale = m[i + j * W]) == 0; j++);
			if (scale == 0) continue;
			for (int i2 = 0; i2 < W; i2++) {
				m[i2 + j * W] /= scale;
				inverse[i2 + j * W] /= scale;
			}
			for (int k = 0; k < W; k++) {
				if (k == j) continue;
				T scale2 = m[i + k * W];
				if (scale2 != 0) {
					for (int i2 = 0; i2 < W; i2++) {
						m[i2 + k * W] -= m[i2 + j * W] * scale2;
						inverse[i2 + k * W] -= inverse[i2 + j * W] * scale2;
					}
				}
			}
		}

		*this = inverse;

		return *this;
	}

	template<typename T, int H, int W>
	template<typename _>
	constexpr Matrix<T, H, W> Matrix<T, H, W>::invert() const {

		Matrix<T, H, W> inverse = Matrix<T, H, W>::Identity();
		Matrix<T, H, W> mtemp(m);

		for (int i = 0; i < W; i++) {
			int j;
			T scale;
			for (j = i; j < W && (scale = mtemp[i + j * W]) == 0; j++);
			if (scale == 0) continue;
			for (int i2 = 0; i2 < W; i2++) {
				mtemp[i2 + j * W] /= scale;
				inverse[i2 + j * W] /= scale;
			}
			for (int k = 0; k < W; k++) {
				if (k == j) continue;
				T scale2 = mtemp[i + k * W];
				if (scale2 != 0) {
					for (int i2 = 0; i2 < W; i2++) {
						mtemp[i2 + k * W] -= mtemp[i2 + j * W] * scale2;
						inverse[i2 + k * W] -= inverse[i2 + j * W] * scale2;
					}
				}
			}
		}

		return inverse;
	}

	template<typename T, int H, int W>
	template<typename _>
	constexpr Matrix<T, H, W> Matrix<T, H, W>::invert(const Matrix<T, H, W> & m) {

		Matrix<T, H, W> inverse = Matrix<T, H, W>::Identity();
		Matrix<T, H, W> mtemp(m);

		for (int i = 0; i < W; i++) {
			int j;
			T scale;
			for (j = i; j < W && (scale = mtemp[i + j * W]) == 0; j++);
			if (scale == 0) continue;
			for (int i2 = 0; i2 < W; i2++) {
				mtemp[i2 + j * W] /= scale;
				inverse[i2 + j * W] /= scale;
			}
			for (int k = 0; k < W; k++) {
				if (k == j) continue;
				T scale2 = mtemp[i + k * W];
				if (scale2 != 0) {
					for (int i2 = 0; i2 < W; i2++) {
						mtemp[i2 + k * W] -= mtemp[i2 + j * W] * scale2;
						inverse[i2 + k * W] -= inverse[i2 + j * W] * scale2;
					}
				}
			}
		}

		return inverse;
	}

	template<typename T, int H, int W>
	constexpr Matrix<T, H, W> Matrix<T, H, W>::lerp(const Matrix<T, H, W> & left, const Matrix<T, H, W> & right, const float k) {

		Matrix<T, H, W> mTemp;

		for (int i = 0; i < W*H; i++) {
			mTemp[i] = left[i] + (right[i] - left[i]) * k;
		}

		return mTemp;
	}

	template<typename T, int H, int W>
	constexpr Matrix<T, H, W>::operator T * () {
		return this->m.data();
	}

	template<typename T, int H, int W>
	constexpr Matrix<T, H, W>::operator const T * () const {
		return this->m.data();
	}

	template<typename T, int W>
	T subDeterminant(const Matrix<T, W, W> & m, std::array<bool, W> x, int y) {

		T result = 0;
		int s = 1;
		for (int i = 0; i < W; i++) {
			if (!x[i]) {
				if (W - y < 2) return m(i, y);
				if (m(i, y)) {
					std::array<bool, W> x2 = x;
					x2[i] = true;
					float sub = subDeterminant(m, x2, y + 1);
					result += s * m(i, y) * sub;
				}
				s *= -1;
			}
		}

		return result;
	}

	template<typename T, int H, int W>
	template<typename _>
	inline constexpr T Matrix<T, H, W>::operator()() const {
		return subDeterminant(*this, std::array<bool, W>{false}, 0);
	}

	template<typename T, int H, int W>
	inline constexpr T & Matrix<T, H, W>::operator()(int x, int y) {
		return m[x + W * y];
	}

	template<typename T, int H, int W>
	inline constexpr const T Matrix<T, H, W>::operator()(int x, int y) const {
		return m[x + W * y];
	}

	template<typename T, int H, int W>
	inline bool Matrix<T, H, W>::operator==(const Matrix & other) const {
		return m == other.m;
	}

	using Matrix2f = Matrix<float, 2>;
	using Matrix3f = Matrix<float, 3>;
	using Matrix4f = Matrix<float, 4>;
	using Matrix2d = Matrix<double, 2>;
	using Matrix3d = Matrix<double, 3>;
	using Matrix4d = Matrix<double, 4>;

}

#ifdef M_PI_DEF
#undef M_PI
#undef M_PI_DEF
#endif
#endif
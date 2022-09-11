#ifndef _VEC
#define _VEC

#include<array>

template<typename T = float, int N = 3>
class Vec {

	std::array<T, N> data;

public:

	// default vector 0, 0, 0
	Vec();
	Vec(const std::array<T, N> &in);

	// component wise addition
	Vec operator+(const Vec other) const;
	// component wise addition
	Vec &operator+=(const Vec other);

	// component wise subtraction
	Vec operator-(const Vec other) const;
	// component wise subtraction
	Vec &operator-=(const Vec other);

	// negation
	Vec operator-() const;

	// dot product
	T operator*(const Vec other) const;

	// scalar product
	Vec operator*(const T scale) const;
	// scalar product
	Vec &operator*=(const T scale);

	// scalar division
	Vec operator/(const T scale) const;
	// scalar division
	Vec &operator/=(const T scale);

	T &operator[](const int i);
	const T &operator[](const int i) const;

	// 1 = magnitude, 2 = square magnitude, any other values return -1
	T operator()(int i = 1);

	// lerp between two vectors
	// * does not preserve scale when lerping between rotations!
	static Vec lerp(Vec a, Vec b, T k);

	operator std::array<T, N>() const;

};

// cross product
template<typename T, int N>
Vec<T, N> operator^(const Vec<T, N> a, const Vec<T, N> b);
// cross product
template<typename T, int N>
T operator^(const Vec<T, N> a, const Vec<T, N> b);
// cross product
template<typename T, int N>
Vec<T, N> &operator^=(Vec<T, N> &a, const Vec<T, N> b);
// cross product
template<typename T, int N>
T &operator^=(Vec<T, N> &a, const Vec<T, N> b);




#define FOR_EACH(a) for (int i = 0; i < N; i++) {a}

template<typename T, int N>
Vec<T, N>::Vec() : data{} {}

template<typename T, int N>
inline Vec<T, N>::Vec(const std::array<T, N>& in) : data(in) {}

template<typename T, int N>
Vec<T, N> Vec<T, N>::operator+(const Vec<T, N> other) const {
	std::array<T, N> arr;
	FOR_EACH(arr[i] = data[i] + other[i];);
	return arr;
}

template<typename T, int N>
Vec<T, N> & Vec<T, N>::operator+=(Vec<T, N> other) {
	FOR_EACH(data[i] += other[i];);
	return *this;
}

template<typename T, int N>
Vec<T, N> Vec<T, N>::operator-(const Vec<T, N> other) const {
	std::array<T, N> arr;
	FOR_EACH(arr[i] = data[i] - other[i];);
	return arr;
}

template<typename T, int N>
Vec<T, N> & Vec<T, N>::operator-=(const Vec<T, N> other) {
	FOR_EACH(data[i] -= other[i];);
	return *this;
}

template<typename T, int N>
Vec<T, N> Vec<T, N>::operator-() const {
	std::array<T, N> arr;
	FOR_EACH(arr[i] = -data[i];);
	return arr;
}

template<typename T, int N>
T Vec<T, N>::operator*(const Vec<T, N> other) const {
	T accumulator = 0;
	FOR_EACH(accumulator += data[i] * other[i];);
	return accumulator;
}

template<typename T, int N>
Vec<T, N> Vec<T, N>::operator*(const T scale) const {
	std::array<T, N> arr;
	FOR_EACH(arr[i] = data[i] * scale;);
	return arr;
}

template<typename T, int N>
Vec<T, N> & Vec<T, N>::operator*=(const T scale) {
	FOR_EACH(data[i] *= scale;);
	return *this;
}

template<typename T, int N>
Vec<T, N> Vec<T, N>::operator/(const T scale) const {
	std::array<T, N> arr;
	T inv = (T)1.0 / scale;
	FOR_EACH(arr[i] = data[i] * inv;);
	return arr;
}

template<typename T, int N>
Vec<T, N> & Vec<T, N>::operator/=(const T scale) {
	T inv = (T)1.0 / scale;
	FOR_EACH(data[i] *= inv;);
	return *this;
}

template<typename T>
Vec<T, 3> operator^(const Vec<T, 3> a, const Vec<T, 3> b) {
	return Vec<T, 3>({ a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0] });
}

template<typename T>
T operator^(const Vec<T, 2> a, const Vec<T, 2> b) {
	return a[0] * b[1] - a[1] * b[0];
}

template<typename T>
Vec<T, 3> & operator^=(Vec<T, 3> &a, const Vec<T, 3> b) {
	Vec<T, 3> temp(a);
	a[0] = temp[1] * b[2] - temp[2] * b[1];
	a[1] = temp[2] * b[0] - temp[0] * b[2];
	a[2] = temp[0] * b[1] - temp[1] * b[0];
	return a;
}

template<typename T, int N>
T &Vec<T, N>::operator[](const int i) {
	return data[i];
}

template<typename T, int N>
const T &Vec<T, N>::operator[](const int i) const {
	return data[i];
}

template<typename T, int N>
T Vec<T, N>::operator()(int i) {
	T sqrMag = *this * *this;
	if (i == 2) return sqrMag;
	if (i == 1) return sqrt(sqrMag);
	return -1;
}

template<typename T, int N>
Vec<T, N> Vec<T, N>::lerp(Vec<T, N> a, Vec<T, N> b, T k) {
	return Vec<T, N>({ a[0] + k * (b[0] - a[0]), a[1] + k * (b[1] - a[1]) , a[2] + k * (b[2] - a[2]) });
}

template<typename T, int N>
Vec<T, N>::operator std::array<T, N>() const {
	return data;
}

template<typename T, int N>
bool operator == (Vec<T, N> left, Vec<T, N> right) {
	for (int i = 0; i < N; i++) {
		if (left[i] != right[i]) return false;
	}
	return true;
}

template<typename T, int N>
bool operator != (Vec<T, N> left, Vec<T, N> right) {
	return !(left == right);
}



using Vec2i = Vec<int, 2>;
using Vec3i = Vec<int, 3>;
using Vec4i = Vec<int, 4>;
using Vec2f = Vec<float, 2>;
using Vec3f = Vec<float, 3>;
using Vec4f = Vec<float, 4>;
using Vec2d = Vec<double, 2>;
using Vec3d = Vec<double, 3>;
using Vec4d = Vec<double, 4>;

#endif
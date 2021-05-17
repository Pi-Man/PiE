#ifndef _VEC3F
#define _VEC3F

#include<array>

class Vec3f {

	std::array<float, 3> data;

public:

	// default vector 0, 0, 0
	Vec3f();
	Vec3f(std::array<float, 3> in);

	// component wise addition
	Vec3f operator+(const Vec3f other) const;
	// component wise addition
	Vec3f &operator+=(const Vec3f other);

	// component wise subtraction
	Vec3f operator-(const Vec3f other) const;
	// component wise subtraction
	Vec3f &operator-=(const Vec3f other);

	// negation
	Vec3f operator-() const;

	// dot product
	float operator*(const Vec3f other) const;

	// scalar product
	Vec3f operator*(const float scale) const;
	// scalar product
	Vec3f &operator*=(const float scale);

	// scalar division
	Vec3f operator/(const float scale) const;
	// scalar division
	Vec3f &operator/=(const float scale);

	// cross product
	Vec3f operator^(const Vec3f other) const;
	// cross product
	Vec3f &operator^=(const Vec3f other);

	float &operator[](const int i);
	const float &operator[](const int i) const;

	// 1 = magnitude, 2 = square magnitude
	float operator()(int pwr = 1);

	// lerp between two vectors
	// * does not preserve scale when lerping between rotations!
	static Vec3f lerp(Vec3f a, Vec3f b, float k);

	operator std::array<float, 3>() const;

};

#endif
#ifndef _MATRIX4F
#define _MATRIX4F

#include<array>

#include "Vec3f.h"


// 4x4 matrix filled with floats
// * as well as being a generic 4x4 matrix, this class also contains 3D transformation methods
class Matrix4f {
	std::array<float, 16> m;
public:

	// creates identity matrix by default
	Matrix4f();
	Matrix4f(const std::array<float, 16>& input);
	~Matrix4f();

	static Matrix4f identity;

	// get value by single index left to right top to botom (0-15)
	float& operator[] (int index);
	// get value by single index left to right top to botom (0-15)
	const float &operator[] (int index) const;

	// matrix multiplication: this * other
	Matrix4f operator* (const Matrix4f& other) const;
	// matrix multiplication: this * other
	Matrix4f& operator*= (const Matrix4f& other);

	// scalar multiplication
	Matrix4f operator* (const float& other) const;
	// scalar multiplication
	Matrix4f& operator*= (const float& other);

	// direction multiplication this * other
	Vec3f operator* (const Vec3f &other) const;

	// add a translation in the X direction
	Matrix4f& translateX(const float distance);
	// add a translation in the Y direction
	Matrix4f& translateY(const float distance);
	// add a translation in the Z direction
	Matrix4f& translateZ(const float distance);
	// add a translation along a vector
	Matrix4f& translate(const float distanceX, const float distanceY, const float distanceZ);
	// add a translation along a vector
	Matrix4f& translate(const Vec3f distance);

	// rotate by degrees around the X axis
	Matrix4f& rotateXd(const float degrees);
	// rotate by radians around the X axis
	Matrix4f& rotateXr(const float radians);
	// rotate by degrees around the Y axis
	Matrix4f& rotateYd(const float degrees);
	// rotate by radians around the Y axis
	Matrix4f& rotateYr(const float radians);
	// rotate by degrees around the Z axis
	Matrix4f& rotateZd(const float degrees);
	// rotate by radians around the Z axis
	Matrix4f& rotateZr(const float radians);

	// scale along the X axis
	Matrix4f& scaleX(const float scale);
	// scale along the Y axis
	Matrix4f& scaleY(const float scale);
	// scale along the Z axis
	Matrix4f& scaleZ(const float scale);
	// uniform scale
	Matrix4f& scale(const float scale);
	// scale along a vector
	Matrix4f& scale(const float scaleX, const float scaleY, const float scaleZ);
	// scale along a vector
	Matrix4f& scale(const Vec3f scale);

	// transpose the matrix
	Matrix4f& transpose();
	// get a transpose of the matrix (copies)
	static Matrix4f transpose(const Matrix4f& m);

	// invert the matrix
	Matrix4f& invert();
	// get an inverse of the matrix (copies)
	Matrix4f invert() const;
	// get an inverse of the matrix (copies)
	static Matrix4f invert(const Matrix4f& m);

	// direcly lerp between two matrices
	// * does not preserve scale when lerping between rotations!
	static Matrix4f lerp(const Matrix4f& left, const Matrix4f &right, const float k);

	operator const float*();

};

#endif
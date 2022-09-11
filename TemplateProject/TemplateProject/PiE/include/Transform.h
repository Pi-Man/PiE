#ifndef _TRANSFORM
#define _TRANSFORM

#include "Matrix.h"

// a wrapper around Matrix4f to provide parenting transformations together
struct Transform {
	// a reference to the transformation that holds the state from the last frame
	Transform *prev = nullptr;
	// a reference to the parent transformation
	Transform *parent = nullptr;
	// the matrix that this transformation wraps
	Matrix4f m = Matrix4f::Identity();

	// the location of the transformation relative to the parent transformation
	Vec3f localPos() const;
	// the location of the transformation relative to global space
	Vec3f globalPos() const;

	operator Matrix4f();
	operator Matrix4f() const;

};

#endif
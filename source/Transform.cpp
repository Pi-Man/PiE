#include "Transform.h"

namespace PiE {

	Vec3f Transform::localPos() const {
		return Vec3f({ m[3], m[7], m[11] });
	}

	Vec3f Transform::globalPos() const {
		Matrix4f global = *this;
		return Vec3f({ global[3], global[7], global[11] });
	}

	Transform::operator Matrix4f() {
		return parent ? (Matrix4f)*parent * m : m;
	}

	Transform::operator Matrix4f() const {
		return parent ? (Matrix4f)*parent * m : m;
	}

}
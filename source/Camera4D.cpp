#include "Camera4D.h"

static Vec4f _pos;
static bool _posDirty = true;

template<>
void Camera4D::rotate4D<AXIS::Y, AXIS::Z>(float amount) {
	pitch4D += amount;
}

template<>
void Camera4D::rotate4D<AXIS::Z, AXIS::Y>(float amount) {
	pitch4D -= amount;
}

void Camera4D::move(float x, float y, float z) {
	viewMatrix3D.translate({ {-x, -y, -z} });
}

void Camera4D::move4D(float x, float y, float z, float w) {
	viewMatrix4D.translate({ {-x, -y, -z, -w} });
}

void Camera4D::rotate(float yaw, float pitch, float roll) {
	viewMatrix3D.rotateD<AXIS::Z, AXIS::X>(yaw);
	pitch3D += pitch;
	viewMatrix3D.rotateD<AXIS::X, AXIS::Y>(roll);
}

const Vec3f Camera4D::getPos() const {
	Vec4f pos = getPos4D();
	return { { pos[0], pos[1], pos[2] } };
}

const Vec4f Camera4D::getPos4D() const {
	if (_posDirty) {
		Matrix<float, 5> inverse = get4DViewMatrix().invert();
		_pos = { { inverse(4, 0), inverse(4, 1), inverse(4, 2), inverse(4, 3) } };
		_posDirty = false;
	}
	return _pos;
}

const Matrix4f Camera4D::getViewMatrix() const {
	return Matrix4f::Identity().rotateD<AXIS::Y, AXIS::Z>(-pitch3D) * viewMatrix3D;
}

const Matrix<float, 5> Camera4D::get4DViewMatrix() const {
	return Matrix<float, 5>::Identity().rotateD<AXIS::Y, AXIS::Z>(-pitch4D) * viewMatrix4D;
}

void Camera4D::set4DViewMatrix(const Matrix<float, 5>& matrix) {
	viewMatrix4D = matrix;
}

VertexArrayObject Camera4D::process4DModel(const Model4D & model) const {
	VertexArrayObject VAO(0);
	Matrix<float, 5> view = get4DViewMatrix();
	//view.invert();
	for (const Vec4f &v : model.points) {
		Vec4f v2 = view * model.matrix * v;
		//if (v2[3] < 0.01) continue;
		Vec3f v3 = Vec3f({ v2[0], v2[1], v2[2] }) / v2[3];
		VAO.buffer.push_back(v3[0]);
		VAO.buffer.push_back(v3[1]);
		VAO.buffer.push_back(v3[2]);
	}
	for (size_t i : model.indices) {
		VAO.indicies.push_back((GLuint)i);
	}
	return VAO;
}
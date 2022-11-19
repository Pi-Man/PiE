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
		Matrix<double, 5> inverse = get4DViewMatrix().invert();
		_pos = { { (float)inverse(4, 0), (float)inverse(4, 1), (float)inverse(4, 2), (float)inverse(4, 3) } };
		_posDirty = false;
	}
	return _pos;
}

const Matrix4f Camera4D::getViewMatrix() const {
	return Matrix4f::Identity().rotateD<AXIS::Y, AXIS::Z>(-pitch3D) * viewMatrix3D;
}

const Matrix<double, 5> Camera4D::get4DViewMatrix() const {
	return Matrix<double, 5>::Identity().rotateD<AXIS::Y, AXIS::Z>(-pitch4D) * viewMatrix4D;
}

void Camera4D::set4DViewMatrix(const Matrix<double, 5>& matrix) {
	viewMatrix4D = matrix;
}

void Camera4D::process4DModel(VertexArrayObject & VAO, const Model4D & model) const {
	Matrix<double, 5> view = get4DViewMatrix();
	//view.invert();
	size_t buffer_index = 0;
	if (VAO.buffer.size() == 0) {
		VAO.buffer.assign(model.points.size() * VAO.format.stride, 0);
	}
	for (const Vertex &vertex : model.points) {
		std::vector<GLdouble> pos = vertex.getAttributeAs<GLdouble>(vertex.format.position_index);
		Vec4d v({ pos[0], pos[1], pos[2], pos[3] });
		Vec4d v2 = view * model.matrix * v;
		//if (v2[3] < 0.01) continue;
		Vec3d v3 = Vec3d({ v2[0], v2[1], v2[2] }) / v2[3];
		Vertex newVert(VAO.format);
		newVert.setAttribute<double>(VAO.format.position_index, { v3[0], v3[1], v3[2] });
		memmove(VAO.buffer.data() + buffer_index, newVert.data, VAO.format.stride);
		buffer_index += VAO.format.stride;
	}
	if (VAO.indicies.size() == 0) {
		for (size_t i : model.indices) {
			VAO.indicies.push_back((GLuint)i);
		}
	}
	VAO.bindBuffers();
}
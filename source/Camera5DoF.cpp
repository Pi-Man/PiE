#include "Camera5DoF.h"

Camera5DoF::Camera5DoF(SDL_Window * window) : Camera(window) {}

void Camera5DoF::move(float x, float y, float z) {
	std::lock_guard<std::mutex> lock(mutex);
	viewMatrix[3] -= x;
	viewMatrix[7] -= y;
	viewMatrix[11] -= z;
	_posDirty = true;
}

void Camera5DoF::rotate(float yaw, float pitch, float _) {
	std::lock_guard<std::mutex> lock(mutex);
	viewMatrix.rotateYd(-yaw);
	this->pitch += pitch;
	if (this->pitch > 89.9) {
		this->pitch = 89.9;
	}
	if (this->pitch < -89.9) {
		this->pitch = -89.9;
	}
}

Matrix4f Camera5DoF::getViewMatrix() {
	std::lock_guard<std::mutex> lock(mutex);
	Matrix4f temp = viewMatrix;
	temp.rotateXd(-pitch);
	return temp;
}

Vec3f Camera5DoF::getPos() {
	std::lock_guard<std::mutex> lock(mutex);
	if (_posDirty) {
		Matrix4f inverse = Matrix4f::invert(viewMatrix);
		_pos = Vec3f ({inverse[3], inverse[7], inverse[11]});
		_posDirty = false;
	}
	return _pos;
}

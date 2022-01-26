#include "Camera.h"

Camera::Camera(SDL_Window * window) : window(window) {}

const Matrix4f Camera::getProjectionMatrix() const {
	//std::lock_guard<std::mutex> lock(mutex);

	float f = farPlane;
	float n = nearPlane;

	int w, h;
	SDL_GetWindowSize(window, &w, &h);

	Matrix4f temp;
	if (!orthogonal) {
		float fov = FOV;
		temp = Matrix4f({
			fov, 0, 0, 0,
			0, fov * (float)w / (float)h, 0, 0,
			0, 0, (f + n) / (f - n), (2 * f*n) / (n - f),
			0, 0, 1, 0
			});
	}
	else {
		float fov = 1 / FOV;
		temp = Matrix4f({
			fov, 0, 0, 0,
			0, fov * (float)w / (float)h, 0, 0,
			0, 0, 2 / (f - n), (f + n) / (n - f),
			0, 0, 0, 1
			});
	}

	return temp;
}

const Matrix4f Camera::getPrevViewMatrix() const {
	//std::lock_guard<std::mutex> lock(mutex);
	return prevViewMatrix;
}

const Matrix4f Camera::getPrevProjectionMatrix() const {
	//std::lock_guard<std::mutex> lock(mutex);
	return prevProjMatrix;
}

void Camera::setClipPlanes(float nearPlane, float farPlane) {
	std::lock_guard<std::mutex> lock(mutex);
	this->nearPlane = nearPlane;
	this->farPlane = farPlane;
}

void Camera::setFOVdeg(float FOV) {
	std::lock_guard<std::mutex> lock(mutex);
	this->FOV = tan(FOV * (float)M_PI / 180.0f);
}

void Camera::setFOVrad(float FOV) {
	std::lock_guard<std::mutex> lock(mutex);
	this->FOV = tan(FOV);
}

float Camera::getFOVdeg() {
	std::lock_guard<std::mutex> lock(mutex);
	return 180.0f / (float)M_PI * atan(FOV);
}

float Camera::getFOVrad() {
	std::lock_guard<std::mutex> lock(mutex);
	return atan(FOV);
}

void Camera::savePreviousState() {
	prevViewMatrix = getViewMatrix();
	prevProjMatrix = getProjectionMatrix();
}

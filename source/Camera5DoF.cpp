#include "Camera5DoF.h"

namespace PiE {

	Camera5DoF::Camera5DoF(SDL_Window * window) : Camera(window) {}

	void Camera5DoF::move(float x, float y, float z) {
		std::lock_guard<std::mutex> lock(mutex);
		viewMatrix[3] -= x;
		viewMatrix[7] -= y;
		viewMatrix[11] -= z;
	}

	void Camera5DoF::rotate(float yaw, float pitch, float _) {
		std::lock_guard<std::mutex> lock(mutex);
		viewMatrix.rotateD<2, 0>(-yaw);
		this->pitch += pitch;
		if (this->pitch > 89.9f) {
			this->pitch = 89.9f;
		}
		if (this->pitch < -89.9f) {
			this->pitch = -89.9f;
		}
	}

	const Matrix4f Camera5DoF::getViewMatrix() const {
		//std::lock_guard<std::mutex> lock(mutex);
		Matrix4f temp = viewMatrix;
		temp.rotateD<1, 2>(-pitch);
		return temp;
	}

	const Vec3f Camera5DoF::getPos() const {
		return pos;
	}

}
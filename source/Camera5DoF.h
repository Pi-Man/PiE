#ifndef _CAMERA_5DOF_H
#define _CAMERA_5DOF_H

#include "Camera.h"

namespace PiE {

// Camera with 5 degrees of freedom, 3 translational (X, Y, Z) and two rotational (yaw, pitch)
	struct Camera5DoF : public Camera {

		float pitch = 0.0f;
		float yaw = 0.0f;
		Vec3f pos = { {0, 0, 0} };

		Camera5DoF(SDL_Window * window);

		void move(float x, float y, float z) override;
		void rotate(float yaw, float pitch = 0, float _ = 0) override;

		const Matrix4f getViewMatrix() const override;

		const Vec3f getPos() const override;
	};

}

#endif
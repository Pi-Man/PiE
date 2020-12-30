#ifndef _CAMERA5DOF
#define _CAMERA5DOF

#include "Camera.h"

// Camera with 5 degrees of freedom, 3 translational (X, Y, Z) and two rotational (yaw, pitch)
class Camera5DoF : public Camera {

	float pitch = 0.0f;

public:

	Camera5DoF(SDL_Window *window);
	
	void move(float x, float y, float z) override;
	void rotate(float yaw, float pitch = 0, float _ = 0) override;

	Matrix4f getViewMatrix() override;

	Vec3f getPos() override;
};

#endif
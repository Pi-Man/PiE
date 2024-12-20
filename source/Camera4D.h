#ifndef _CAMERA_4D_H
#define _CAMERA_4D_H

#include "Camera.h"
#include "Model4D.h"
#include "VertexArrayObject.h"

namespace PiE {
	struct Camera4D : public Camera {
		Matrix<double, 5> viewMatrix4D = Matrix<double, 5>::Identity();
		Matrix4f viewMatrix3D = Matrix4f::Identity();
		Vec4f pos;
		float pitch4D = 0;
		float pitch3D = 0;

		Camera4D(SDL_Window * window) : Camera(window) {}

		void move(float x, float y, float z) override;

		void move4D(float x, float y, float z, float w);

		void rotate(float yaw, float pitch = 0, float roll = 0) override;

		template<int AXIS_1, int AXIS_2>
		void rotate4D(float amount);

		const Vec3f getPos() const override;

		const Vec4f getPos4D() const;

		const Matrix4f getViewMatrix() const override;

		const Matrix<double, 5> get4DViewMatrix() const;

		void set4DViewMatrix(const Matrix<double, 5> & matrix);

		void process4DModel(VertexArrayObject & VAO, const Model4D & model) const;
	};

	template<>
	void Camera4D::rotate4D<AXIS::Y, AXIS::Z>(float amount);

	template<>
	void Camera4D::rotate4D<AXIS::Z, AXIS::Y>(float amount);

	template<int AXIS_1, int AXIS_2>
	void Camera4D::rotate4D(float amount) {
		viewMatrix4D.rotateD<AXIS_1, AXIS_2>(-amount);
	}
}

#endif
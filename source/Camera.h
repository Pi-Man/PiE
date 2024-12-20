#ifndef _CAMERA_H
#define _CAMERA_H

#include <mutex>
#include <cmath>

#include <SDL.h>

#include "Matrix.h"

namespace PiE {

// abstract camera class
// * currently the only one camera type is added (Camera5DoF) however you can extend this class yourself if needed
	struct Camera {

		std::mutex mutex;
		Matrix4f viewMatrix = Matrix4f::Identity(), prevViewMatrix;
		Matrix4f projMatrix, prevProjMatrix;
		SDL_Window * window;

		// if the camera should use an orthogonal projection matrix
		bool orthogonal = false;

		// the near clip plane of the camera
		float nearPlane = 0.125f;
		// the far clip plane of the camera
		float farPlane = 256.0f;
		// the field of view of the camera
		// * for orthogonal projection matrix, this is the half width of the camera view
		// * for perspective projection matrix, this is the "slope" of the camera angle.  ie: the half width of the camera view at one unit away from the camera
		float FOV = 1;

		// the window reference is necessary for properly creating the projection matrix a the correct aspect ratio
		Camera(SDL_Window * window);

		// move the camera along a vector
		virtual void move(float x, float y, float z) = 0;
		// rotate the camera along the three axis
		// * it is up to the child class to define in what order and how these rotations are applied
		virtual void rotate(float yaw, float pitch = 0, float roll = 0) = 0;

		// get the view matrix of the camera
		virtual const Matrix4f getViewMatrix() const = 0;
		// get the projection matrix of the camera
		const Matrix4f getProjectionMatrix() const;
		// get the view matrix of the camera from the previous tick (only populated if render interpolation is active)
		const Matrix4f getPrevViewMatrix() const;
		// get the projection matrix of the camera from the previous tick (only populated if render interpolation is active)
		const Matrix4f getPrevProjectionMatrix() const;

		// get the current position of the camera
		virtual const Vec3f getPos() const = 0;

		// set the clip planes of the camera
		void setClipPlanes(float nearPlane = 0.125f, float farPlane = 256.0f);
		// set the field of view to an angle in degrees (angle from center to edge)
		void setFOVdeg(float FOV);
		// set the field of view to an angle in radians (angle from center to edge)
		void setFOVrad(float FOV);
		// get the field of view as an angle in degrees (angle from center to edge)
		float getFOVdeg();
		// get the field of view as an angle in radians (angle from center to edge)
		float getFOVrad();

		void savePreviousState();
	};
}
#endif
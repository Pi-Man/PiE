#ifndef _SPOT_LIGHT
#define _SPOT_LIGHT

#include "Vec.h"

// directional light info used to populate shader uniforms
// * lights are sorted by intensity
struct SpotLight {

	// position of the light
	// * default: 0, 0, 0
	Vec3f pos;

	// direction of the light
	// * default: 0.57735, 0.57735, 0.57735
	Vec3f dir = Vec3f({ 0.57735f, 0.57735f, 0.57735f });

	// color of the light
	// * default: 1, 1, 1
	Vec3f col = Vec3f({ 1.0f, 1.0f, 1.0f });

	// intensity of the light
	// * default: 0
	float intensity = 0;

	// cos of the angle of coverage
	// * default 45 degrees
	float angle = 0.70721;

	// position of the light from the previous tick (only populated if render interpolation is active)
	Vec3f prevPos;

	// direction of the light from the previous tick (only populated if render interpolation is active)
	Vec3f prevDir = Vec3f({ 0.57735f, 0.57735f, 0.57735f });

	// color of the light from the previous tick (only populated if render interpolation is active)
	Vec3f prevCol = Vec3f({ 1.0f, 1.0f, 1.0f });

	// intensity of the light from the previous tick (only populated if render interpolation is active)
	float prevIntensity = 0;

	// angle of the light from the previous tick (only populated if render interpolation is active)
	float prevAngle = 0.70721;

};

#endif
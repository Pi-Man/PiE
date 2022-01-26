#ifndef _DIRECTIONAL_LIGHT
#define _DIRECTIONAL_LIGHT

#include "Vec.h"

// directional light info used to populate shader uniforms
// * lights are sorted by intensity
struct DirectionalLight {

	// direction of the light
	// * default: 0.57735, 0.57735, 0.57735
	Vec3f dir = Vec3f({ 0.57735f, 0.57735f, 0.57735f });

	// color of the light
	// * default: 1, 1, 1
	Vec3f col = Vec3f({ 1.0f, 1.0f, 1.0f });

	// intensity of the light
	// * default: 2
	float intensity = 2.0f;

	// intensity of the light from the previous tick (only populated if render interpolation is active)
	Vec3f prevDir = Vec3f({ 0.57735f, 0.57735f, 0.57735f });

	// color of the light from the previous tick (only populated if render interpolation is active)
	Vec3f prevCol = Vec3f({ 1.0f, 1.0f, 1.0f });

	// intensity of the light from the previous tick (only populated if render interpolation is active)
	float prevIntensity = 2;

};

#endif
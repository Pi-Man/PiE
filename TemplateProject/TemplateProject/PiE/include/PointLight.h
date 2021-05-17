#ifndef _POINT_LIGHT
#define _POINT_LIGHT

#include "Vec3f.h"


// point light info used to populate shader uniforms
// * lights are sorted per object by intensity divided by distance to the object squared
struct PointLight {

	// position of the light
	// * default: 0, 0, 0
	Vec3f pos;

	// color of the light
	// * default: 1, 1, 1
	Vec3f col = Vec3f({ 1, 1, 1 });

	// intensity of the light
	// * default: 0
	float intensity =  0;

	// position of the light from the previous tick (only populated if render interpolation is active)
	Vec3f prevPos;

	// color of the light from the previous tick (only populated if render interpolation is active)
	Vec3f prevCol = Vec3f({ 1, 1, 1 });

	// intensity of the light from the previous tick (only populated if render interpolation is active)
	float prevIntensity = 0;

};

#endif
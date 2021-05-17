#ifndef _UTILS
#define _UTILS

#include "Vec3f.h"

namespace Utils {

	// move a vector twords another vector a specified distance
	// * from: the starting vector
	// * to: the target vector
	// * distance: maximum distance that can be traveled
	// * intended to be used to move a vector from one position to another over several frames at a specified speed
	Vec3f moveTowards(Vec3f from, Vec3f to, float distance);

	// lerp between two values
	double lerp(double a, double b, double k);
	// lerp between two values
	float lerp(float a, float b, float k);

}



#endif
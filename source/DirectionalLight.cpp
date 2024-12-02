#include "DirectionalLight.h"

inline bool PiE::DirectionalLight::operator==(const DirectionalLight & other) const {
	return dir == other.dir &&  col == other.col &&  intensity == other.intensity &&  prevDir == other.prevDir &&  prevCol == other.prevCol &&  prevIntensity == other.prevIntensity;
}

inline bool PiE::DirectionalLight::operator!=(const DirectionalLight & other) const {
	return !(*this == other);
}

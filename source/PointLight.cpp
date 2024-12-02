#include "PointLight.h"

inline bool PiE::PointLight::operator==(const PointLight & other) const {
	return pos == other.pos &&  col == other.col &&  intensity == other.intensity &&  prevPos == other.prevPos &&  prevCol == other.prevCol &&  prevIntensity == other.prevIntensity;
}

inline bool PiE::PointLight::operator!=(const PointLight & other) const {
	return !(*this == other);
}

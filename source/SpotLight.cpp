#include "SpotLight.h"

namespace PiE {

	inline bool SpotLight::operator==(const SpotLight & other) const {
		return pos == other.pos &&  dir == other.dir &&  col == other.col &&  intensity == other.intensity &&  angle == other.angle &&  prevPos == other.prevPos &&  prevDir == other.prevDir &&  prevCol == other.prevCol &&  prevIntensity == other.prevIntensity &&  prevAngle == other.prevAngle;
	}

	inline bool SpotLight::operator!=(const SpotLight & other) const {
		return !(*this == other);
	}

}
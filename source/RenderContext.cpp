#include "RenderContext.h"

namespace PiE {

	bool RenderContext::operator==(RenderContext & other) const {
		//return !memcmp(this, &other, sizeof(RenderContext));
		return
			texture == other.texture &&
			shader == other.shader &&
			camera == other.camera &&
			cullFace == other.cullFace &&
			blendSRC == other.blendSRC &&
			blendDST == other.blendDST &&
			blendSRC_alpha == other.blendSRC_alpha &&
			blendDST_alpha == other.blendDST_alpha &&
			blendEq == other.blendEq &&
			depthFunc == other.depthFunc &&
			clearColor == other.clearColor &&
			clearDepth == other.clearDepth &&
			depthEnable == other.depthEnable &&
			cullEnable == other.cullEnable &&
			blendEnable == other.blendEnable
			;
	}

	bool RenderContext::operator!=(RenderContext & other) const {
		return !(*this == other);
	}

}
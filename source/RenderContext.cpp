#include "RenderContext.h"

bool RenderContext::operator==(RenderContext &other) const {
	return 
		depthEnable == other.depthEnable && \
		cullFace == other.cullFace && \
		cullEnable == other.cullEnable && \
		blendSRC == other.blendSRC && \
		blendDST == other.blendDST && \
		clearColor == other.clearColor && \
		clearDepth == other.clearDepth && \
		texture == other.texture;
}

bool RenderContext::operator!=(RenderContext &other) const {
	return !(*this == other);
}
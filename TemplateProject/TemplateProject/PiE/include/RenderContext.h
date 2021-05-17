#ifndef _RENDER_CONTEXT
#define _RENDER_CONTEXT

#include <glew.h>

#include "GL_Texture.h"

// describes the values that the GL state should have when rendering the render object
struct RenderContext {

	// whether or not to use the depth buffer
	bool depthBuffer = true;
	// what side of the face to cull
	int cullFace = GL_BACK;
	// whether or not to use face culling
	bool cullEnable = true;
	// whether or not to use blending
	bool blendEnable = true;
	// the source blend function
	int blendSRC = GL_SRC_ALPHA;
	// the destination blend function
	int blendDST = GL_ONE_MINUS_SRC_ALPHA;
	// the color to clear the screen with
	// * will likely get moved to a separate struct later
	unsigned int clearColor = 0xff404040;
	// the depth to clear the depth buffer with
	// * will likely get moved to a separate struct later
	float clearDepth = 1.0f;
	// the texture to use to render with
	GL_Texture2D *texture = nullptr;

	bool operator==(RenderContext &other) const;
	bool operator!=(RenderContext &other) const;
};

#endif
#ifndef _RENDER_CONTEXT_H
#define _RENDER_CONTEXT_H

#include <glew.h>

#include "GL_Texture2D.h"
#include "Camera.h"
#include "Shader.h"

namespace PiE {

// describes the values that the GL state should have when rendering the render object
	struct RenderContext {

		// the texture to use to render with
		GL_Texture2D * texture = nullptr;
		// the camera to render with
		// * default: use the main shader associated with the engine context
		Camera * camera = nullptr;
		// the sader to render with
		// * default: use the main shader associated with the engine context
		Shader * shader = nullptr;
		// what side of the face to cull
		int cullFace = GL_BACK;
		// the source blend function
		int blendSRC = GL_SRC_ALPHA;
		// the destination blend function
		int blendDST = GL_ONE_MINUS_SRC_ALPHA;
		// the source blend function for alpha channel only (only used if both source and destination functions are set, -1 to dissable)
		int blendSRC_alpha = -1;
		// the destination blend function for alpha channel only (only used if both source and destination functions are set, -1 to dissable)
		int blendDST_alpha = -1;
		// constant blend color used with some blend functions
		unsigned int blendColor = COLOR_A(255, 255, 255, 255);
		// blend equation to use
		int blendEq = GL_FUNC_ADD;
		// how the depth of fragments should be tested
		int depthFunc = GL_LESS;
		// the color to clear the screen with
		// * will likely get moved to a separate struct later
		unsigned int clearColor = 0xffb04000;
		// the depth to clear the depth buffer with
		// * will likely get moved to a separate struct later
		float clearDepth = 1.0f;
		// whether or not to use the depth buffer
		bool depthEnable = true;
		// wether or not to write to the depth buffer (if depth buffer is enabled)
		bool depthMask = true;
		// whether or not to use face culling
		bool cullEnable = true;
		// whether or not to use blending
		bool blendEnable = true;
		// if true, renders after solid objects
		bool isTransparent = false;

		bool operator==(RenderContext & other) const;
		bool operator!=(RenderContext & other) const;
	};
}

#endif
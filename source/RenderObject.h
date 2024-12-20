#ifndef _RENDER_OBJECT_H
#define _RENDER_OBJECT_H

#include <SDL.h>

#include "VertexArrayObject.h"
#include "RenderContext.h"
#include "Transform.h"
#include "Shader.h"

namespace PiE {

// an object to be rendered to the screen along with the data needed to render the object
	struct RenderObject {
		// the vertex array object to be rendered
		VertexArrayObject VAO;
		// the transform of the object from the previous frame (only gets populated if render interpolation is enabled)
		Transform prevTransform;
		// the transform of the object to be rendered
		Transform transform;
		// the render context to render with
		RenderContext renderContext;
	};
}

#endif
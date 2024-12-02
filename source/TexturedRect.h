#ifndef _TEXTURED_RECT_H
#define _TEXTURED_RECT_H

#include <string>

#include "Component.h"
#include "Rect.h"
#include "GL_Texture2D.h"
#include "RenderObject.h"
#include "Camera.h"
#include "Shader.h"

namespace PiE {

	struct TexturedRect : Component {

		Camera * camera;

		Shader * shader;

		AxisAlignedRect * bounds;

		RenderObject renderObject;

		GL_Texture2D *& texture;

		Lazy<Matrix4f> m;

		TexturedRect(AxisAlignedRect * bounds, GL_Texture2D * texture, Camera * camera, Shader * shader = nullptr);

		virtual void fixedUpdate(EngineContext & ctx);

		virtual void onAdded(EngineContext & ctx, GameObject & gameObject);

	};

}

#endif
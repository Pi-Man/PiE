#ifndef _TEXTURED_RECT_H_
#define _TEXTURED_RECT_H_

#include <string>

#include "Component.h"
#include "Rect.h"
#include "GL_Texture.h"
#include "RenderObject.h"
#include "Camera.h"
#include "Shader.h"

struct TexturedRect : Component {

	Camera *camera;

	Shader *shader;

	AxisAlignedRect *bounds;

	RenderObject renderObject;

	GL_Texture2D *&texture;

	Lazy<Matrix4f> m;

	TexturedRect(AxisAlignedRect *bounds, GL_Texture2D *texture, Camera *camera, Shader *shader = nullptr);

	virtual void fixedUpdate(PiE::EngineContext &ctx);

	virtual void onAdded(PiE::EngineContext &ctx, GameObject &gameObject);

};

#endif
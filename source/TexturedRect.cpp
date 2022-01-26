#include "TexturedRect.h"
#include "PiE.h"

TexturedRect::TexturedRect(AxisAlignedRect *bounds, GL_Texture2D *texture, Camera *camera, Shader *shader) : bounds(bounds), camera(camera), shader(shader), texture(renderObject.renderContext.texture), m([this]() {
	return this->bounds->createGUIMatrix();
	//return Matrix4f::Identity()
	//	.scale({ {(float)this->bounds->width(), (float)this->bounds->height(), 1.0f, 1.0f} })
	//	.translate({ {(float)this->bounds->min[0](), (float)this->bounds->min[1](), 0} })
	//	.scale({ {2.0f, 2.0f, 1.0f, 1.0f} })
	//	.translate({ {-1.0f, -1.0f, 0.0f} });
}) {

	renderObject.VAO = VertexArrayObject(VAO_UVS);
	renderObject.VAO.addQuadWithUVs({
		0.0, 0.0, 0.0,
			0.0, 1.0,
		1.0, 0.0, 0.0,
			1.0, 1.0,
		1.0, 1.0, 0.0,
			1.0, 0.0,
		0.0, 1.0, 0.0,
			0.0, 0.0
	});
	renderObject.renderContext.texture = texture;
	renderObject.renderContext.shader = shader;
	events.push_back({ SDL_EventType::SDL_WINDOWEVENT, PiE::EventCallback([this](PiE::EngineContext & ctx, SDL_Event event) {
		if (event.window.windowID == SDL_GetWindowID(ctx.mainWindow) && event.window.event == SDL_WindowEventID::SDL_WINDOWEVENT_RESIZED) {
			m.invalidate();
		}
	}) });
}

void TexturedRect::fixedUpdate(PiE::EngineContext & ctx) {
	renderObject.transform.m = (camera->getProjectionMatrix() * camera->getViewMatrix()).invert() * m();
}

void TexturedRect::onAdded(PiE::EngineContext & ctx, GameObject & gameObject) {
	gameObject.renderObjects.push_back(&renderObject);
}

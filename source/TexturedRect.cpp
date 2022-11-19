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
	VertexFormat format{ POSITION_F, UV2_F };
	renderObject.VAO = VertexArrayObject(format);
	renderObject.VAO.addQuad(
		{ format,
		{0.0f, 0.0f, 0.0f},
		{0.0f, 1.0f} },
		{ format,
		{1.0f, 0.0f, 0.0f},
		{1.0f, 1.0f} },
		{ format,
		{1.0f, 1.0f, 0.0f},
		{1.0f, 0.0f} },
		{ format,
		{0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f} }
	);
	renderObject.renderContext.texture = texture;
	renderObject.renderContext.shader = shader;
	events.push_back({ SDL_EventType::SDL_WINDOWEVENT, PiE::EventCallback([this](PiE::EngineContext & ctx, SDL_Event event) {
		if (event.window.windowID == SDL_GetWindowID(ctx.mainWindow) && event.window.event == SDL_WindowEventID::SDL_WINDOWEVENT_RESIZED) {
			m.invalidate();
		}
	}) });
}

void TexturedRect::fixedUpdate(PiE::EngineContext & ctx) {
	renderObject.transform.m = Matrix4f::Identity().scale<AXIS::Y>((float)ctx.windowSize[1] / (float)ctx.windowSize[0]) * m();
}

void TexturedRect::onAdded(PiE::EngineContext & ctx, GameObject & gameObject) {
	gameObject.renderObjects.push_back(&renderObject);
}

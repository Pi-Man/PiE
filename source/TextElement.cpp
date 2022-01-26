#include "TextElement.h"

#include "PiE.h"

TextElement::TextElement(AxisAlignedRect * bounds, Shader * shader) :
bounds(bounds),
m([this]()->Matrix4f {
	return this->bounds->createGUIMatrix();// *Matrix4f::Identity().scale({ {1.0f, this->bounds->getWidth() / this->bounds->getHeight(), 1.0f, 1.0f} });
	//return Matrix4f::Identity()
	//	.scale({ {(float)this->bounds->width(), (float)this->bounds->width(), 1.0f, 1.0f} })
	//	.translate({ {(float)this->bounds->xmin(), (float)this->bounds->ymin(), 0} })
	//	.scale({ {2.0f, 2.0f, 1.0f, 1.0f} })
	//	.translate({ {-1.0f, -1.0f, 0.0f} });
}) {
	textObject.VAO = VertexArrayObject(VAO_UVS);
	textObject.renderContext.shader = shader;
	textObject.renderContext.cullEnable = false;
	events.push_back({ SDL_EventType::SDL_WINDOWEVENT, PiE::EventCallback([this](PiE::EngineContext & ctx, SDL_Event event) {
		if (event.window.windowID == SDL_GetWindowID(ctx.mainWindow) && event.window.event == SDL_WindowEventID::SDL_WINDOWEVENT_RESIZED) {
			update();
			m.invalidate();
		}
	}) });
}

TextElement::TextElement(AxisAlignedRect * bounds, Shader * shader, Font & font, std::string text, bool centerX, bool centerY) : TextElement(bounds, shader) {
	this->centerX = centerX;
	this->centerY = centerY;
	setAndUpdate(font, text);
}

void TextElement::update() {
	RenderObject temp = font->buildRenderObjectWithAttributes(text, (VAO_Flags)0, [this](std::vector<GLfloat> & out, const Font::RenderInfo & info) {

		float width = bounds->getWidth() * bounds->ctx->windowSize[0];
		float height = bounds->getHeight() * bounds->ctx->windowSize[1];

		float scalex = std::max(info.widthMax, (info.top - info.bottom) * width / height);
		float scaley = scalex * height / width;

		out[0] = ((GLfloat)info.vertex[0] / scalex + (centerX ? (1.0f - info.width / scalex) / 2.0f : 0.0f));
		out[1] = (((GLfloat)info.vertex[1] - info.bottom) / scaley + (centerY ? (1.0f - (info.top - info.bottom) / scaley) / 2.0f : 0.0f));
		out[2] = ((GLfloat)info.vertex[2] - 1.0f);

	});
	textObject.VAO.copy(temp.VAO);
}

void TextElement::setAndUpdate(Font & font, std::string text) {
	this->font = &font;
	this->text = text;
	update();
}

void TextElement::fixedUpdate(PiE::EngineContext & ctx) {
	textObject.transform.m = (ctx.mainCamera->getProjectionMatrix() * ctx.mainCamera->getViewMatrix()).invert() * m();
}

void TextElement::onAdded(PiE::EngineContext & ctx, GameObject & gameObject) {
	gameObject.renderObjects.push_back(&textObject);
}

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
	textObject.VAO = VertexArrayObject({ POSITION_F, UV2_F });
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
	RenderObject temp = font->buildRenderObjectWithAttributes(text, { POSITION_F, UV2_F }, [this](Vertex & out, const Font::RenderInfo & info) {

		double width = bounds->getWidth() * bounds->ctx->windowSize[0];
		double height = bounds->getHeight() * bounds->ctx->windowSize[1];

		double scaley = height / width;

		double scale = (info.widthMax / width) > ((info.top - info.bottom) / height) ? info.widthMax : (info.top - info.bottom) / scaley;

		//out[0] = (((GLfloat)info.vertex[0] + (centerX ? (info.widthMax - info.width) / 2.0f : 0.0f)) / scale);
		//out[1] = (((GLfloat)info.vertex[1] - info.bottom) / scale);
		//out[2] = ((GLfloat)info.vertex[2] - 1.0f);

		float x = (((GLfloat)info.vertex[0] + (centerX ? (info.widthMax - info.width) / 2.0f : 0.0f)) / scale) + (centerX ? (1.0f - info.widthMax / scale) / 2.0f : 0.0f);
		float y = (((GLfloat)info.vertex[1] - info.bottom) / (scale * scaley)) + (centerY ? (1.0f - (info.top - info.bottom) / (scale * scaley)) / 2.0f : 0.0f);
		float z = ((GLfloat)info.vertex[2] - 1.0f);

		out.setAttribute<float>(0, { x, y, z });
		out.setAttribute<float>(1, { (float)info.uv[0], (float)info.uv[1] });

	});
	textObject.VAO.copy(temp.VAO);
}

void TextElement::setAndUpdate(Font & font, std::string text) {
	this->font = &font;
	this->text = text;
	update();
}

void TextElement::fixedUpdate(PiE::EngineContext & ctx) {
	textObject.transform.m = Matrix4f::Identity().scale<AXIS::Y>((float)ctx.windowSize[1] / (float)ctx.windowSize[0]) * m();
}

void TextElement::onAdded(PiE::EngineContext & ctx, GameObject & gameObject) {
	gameObject.renderObjects.push_back(&textObject);
}

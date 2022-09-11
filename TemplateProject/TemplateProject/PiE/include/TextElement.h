#pragma once

#include "Component.h"
#include "RenderObject.h"
#include "FontRenderer.h"
#include "Rect.h"

struct TextElement : Component {

	RenderObject textObject;
	AxisAlignedRect * bounds;
	Lazy<Matrix4f> m;

	bool centerX = false;
	bool centerY = false;

	Font * font;
	std::string text = "";

	TextElement(AxisAlignedRect * bounds, Shader * shader);
	TextElement(AxisAlignedRect * bounds, Shader * shader, Font & font, std::string text, bool centerX = false, bool centerY = false);

	void update();
	void setAndUpdate(Font &font, std::string text);

	// called once every logic tick
	virtual void fixedUpdate(PiE::EngineContext &ctx);

	// called once the game object this component is attached to is successfully added
	virtual void onAdded(PiE::EngineContext &ctx, GameObject &gameObject);

};


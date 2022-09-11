#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <unordered_map>

#include <SDL.h>

#include "Rect.h"
#include "Callbacks.h"
#include "RenderObject.h"
#include "Component.h"

struct Clickable : Component {

	AxisAlignedRect *bounds;

	PiE::EventCallback onClick;
	std::unordered_map<size_t, PiE::FixedUpdateCallback> onHeld;
	PiE::EventCallback onRelease;
	PiE::EventCallback onHoverStart;
	PiE::FixedUpdateCallback onHovering;
	PiE::EventCallback onHoverEnd;

	bool hovered = false;
	bool held[255];

	Clickable(AxisAlignedRect *bounds);

	void fixedUpdate(PiE::EngineContext & ctx) override;

};

#endif
#ifndef _CLICKABLE_H
#define _CLICKABLE_H

#include <unordered_map>

#include <SDL.h>

#include "Rect.h"
#include "Callbacks.h"
#include "RenderObject.h"
#include "Component.h"

namespace PiE {

	// defines a rectangular area on the screen that can be clicked on
	// * does not provide any rendering by itself, use with TextElement
	// * or TexturedRect or both for visuals
	struct Clickable : Component {

		// the area on screen that can be clicked, can be shared with TextElement or TexturedRect for visual feedback
		AxisAlignedRect * bounds;

		// called when the area is clicked
		EventCallback onClick;

		// called when the button indicated by the map key is held down every update
		std::unordered_map<size_t, FixedUpdateCallback> onHeld;

		// called when the button is no longer held
		EventCallback onRelease;

		// called when the mouse enters the area
		EventCallback onHoverStart;

		// called every frame that the mouse is in the area
		FixedUpdateCallback onHovering;

		// called when the mouse leaves the area
		EventCallback onHoverEnd;

		bool hovered = false;
		bool held[255];

		Clickable(AxisAlignedRect * bounds);

		void fixedUpdate(EngineContext & ctx) override;

	};

}

#endif
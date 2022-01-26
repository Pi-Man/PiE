#include "Clickable.h"

#include "PiE.h"

Clickable::Clickable(AxisAlignedRect *bounds) : bounds(bounds) {
	memset(held, 0, sizeof(held));


	events.push_back({ SDL_EventType::SDL_MOUSEBUTTONDOWN, PiE::EventCallback([this](PiE::EngineContext &ctx, SDL_Event event) {
		double x = (double)event.button.x / (double)ctx.windowSize[0];
		double y = 1.0 - ((double)event.button.y / (double)ctx.windowSize[1]);
		if (!held[event.button.button] && this->bounds->testPoint({ { x, y } })) {
			held[event.button.button] = true;
			if (onClick.index) onClick.f(ctx, event);
		}
	}) });
	events.push_back({ SDL_EventType::SDL_MOUSEBUTTONUP, PiE::EventCallback([this](PiE::EngineContext &ctx, SDL_Event event) {
		if (held[event.button.button]) {
			held[event.button.button] = false;
			if (onRelease.index) onRelease.f(ctx, event);
		}
	}) });
	events.push_back({ SDL_EventType::SDL_MOUSEMOTION, PiE::EventCallback([this](PiE::EngineContext &ctx, SDL_Event event) {
		double x = (double)event.motion.x / (double)ctx.windowSize[0];
		double y = 1.0 - ((double)event.motion.y / (double)ctx.windowSize[1]);
		if (!hovered && this->bounds->testPoint({ { x, y } })) {
			hovered = true;
			if (onHoverStart.index) onHoverStart.f(ctx, event);
		}
	}) });
	events.push_back({ SDL_EventType::SDL_MOUSEMOTION, PiE::EventCallback([this](PiE::EngineContext &ctx, SDL_Event event) {
		double x = (double)event.button.x / (double)ctx.windowSize[0];
		double y = 1.0 - ((double)event.button.y / (double)ctx.windowSize[1]);
		if (hovered && !this->bounds->testPoint({ { x, y } })) {
			hovered = false;
			if (onHoverEnd.index) onHoverEnd.f(ctx, event);
		}
	}) });
}

void Clickable::fixedUpdate(PiE::EngineContext & ctx) {
	for (std::pair<const size_t, PiE::FixedUpdateCallback> & pair : onHeld) {
		if (held[pair.first]) {
			pair.second.f(ctx);
		}
	}
	if (hovered && onHovering.index) {
		onHovering.f(ctx);
	}
}


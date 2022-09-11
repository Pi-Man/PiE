#ifndef _CALLBACKS
#define _CALLBACKS

#include <functional>

#include <SDL.h>

namespace PiE {
	
	struct EngineContext;

	struct FixedUpdateCallback {
		std::function<void(EngineContext&)> f;
		size_t index = 0;
		static size_t lastIndex;

		FixedUpdateCallback() = default;
		FixedUpdateCallback(std::function<void(EngineContext&)> f);

		bool operator== (FixedUpdateCallback &other);
		bool operator== (const FixedUpdateCallback &other);
	};

	struct EventCallback {
		std::function<void(EngineContext&, SDL_Event)> f;
		size_t index = 0;
		static size_t lastIndex;

		EventCallback() = default;
		EventCallback(std::function<void(EngineContext&, SDL_Event)> f);

		bool operator== (EventCallback &other);
		bool operator== (const EventCallback &other);
	};
}

#endif
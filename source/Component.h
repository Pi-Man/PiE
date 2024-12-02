#ifndef _COMPONENT_H
#define _COMPONENT_H

#include <vector>

#include <SDL.h>

#include "Callbacks.h"

namespace PiE {

	struct GameObject;

	// component to be attached to a game object
	// * the component's event list will be added to the engine context's event list
	// * the component's fixedUpdate method will be called once every tick
	// * the component's onAdded method will be called when the game object the component is attached to is added to the engine context
	// ** though not intended it is possible to attach one component to multiple game objects, this will likely be depricated in the very near future
	// ** in this case the events list will be duplicated and the fixedUpdate and onAdded method will be called once for each game object that this component is attached to
	// ** additionally the game object reference will only reference the last game object that was added to the engine context
	struct Component {

		// a reference to the game object this component is attached to, is populated after the game object is added
		GameObject * gameObject = nullptr;

		// a list of events for the component to listen to
		std::vector<std::pair<Uint32, PiE::EventCallback>> events{};

		// a copy of the update function so it can be deleted from the engine context later
		PiE::FixedUpdateCallback fixedUpdateFunction;

		// called once every logic tick
		virtual void fixedUpdate(PiE::EngineContext & ctx) {};

		// called once the game object this component is attached to is successfully added
		virtual void onAdded(PiE::EngineContext & ctx, GameObject & gameObject) {};
	};

}

#endif
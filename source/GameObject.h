#ifndef _GAME_OBJECT
#define _GAME_OBJECT

#include <vector>

#include "Matrix.h"
#include "Component.h"
#include "VertexArrayObject.h"
#include "RenderObject.h"

// game object to be added to the engine context
// * components is a vector of references to all the compontents that are added to this game object
// * renderObjects is a vector of references to all the render objects that are added to this game object
// * transform is the transform of the game object
// * prevTransform is the transform from the previous tick (only populated if render interpolation is active)
// ** if components is added to after the game engine is added to the engine context then that component will not be "active"
// ** if renderObjects is added to after the game engine is added to the engine context then that render object will not be "active"
// ** though not necessary, it is intended that transform be the parent transform of one or more of the render objects in renderObjects
struct GameObject {

	// list of component references attached to this game object
	std::vector<Component*> components;

	// list of render object references attached to this game object
	std::vector<RenderObject*> renderObjects;

	// transform of this game object
	Transform transform;

	// transform of this game object from the previous tick (only populated if render interpolation is active)
	Transform prevTransform;

};

#endif
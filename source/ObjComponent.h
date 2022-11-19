#ifndef _OBJ_COMPONENT
#define _OBJ_COMPONENT

#include "GameObject.h"
#include "Shader.h"
#include "obj.h"
#include "VertexAttribute.h"

// a premade component that loads a Wavefront OBJ model
// * the model is loaded on onAdded
// * the first render object in parts is parented to the game object, all other render objects are parented to the first render object
struct ObjComponent : public Component {
	Shader shader;
	std::string file;
	VertexFormat format;
	// a list of references to the render objects loaded from the obj file for easy access
	std::vector<RenderObject*> parts;
	// creates a component that loads an obj file to the game objects render objects and sets the parent of the obj model to the game object
	// * file: the path/file of the model
	// * shader: the shader to use for the render objects
	ObjComponent(std::string file, Shader shader, VertexFormat format);
	~ObjComponent();
	void onAdded(PiE::EngineContext &ctx, GameObject &gameObject);
};
#endif
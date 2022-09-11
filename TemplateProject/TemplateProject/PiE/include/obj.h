#ifndef _OBJ
#define _OBJ

#include <map>

#include <SDL.h>

#include "VertexArrayObject.h"
#include "RenderObject.h"
#include "GL_Texture.h"

// namespace for loading Wavefront obj models
namespace OBJ {

	struct Material {
		RenderContext renderContext{};
		Vec3f textureOffset;
		Vec3f textureScale;
	};

	// default material
	extern RenderContext blankMtl;
	// parse a material library file
	std::map<std::string, RenderContext> parseMtl(const char *fileName);
	// parse an obj file
	// * flags: the flags to initialize the vertex array objects with
	std::vector<RenderObject> parseObj(const char *fileName, Uint32 flags = VAO_NORMALS | VAO_UVS);
}

#endif
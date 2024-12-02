#ifndef _OBJ_H
#define _OBJ_H

#include <map>

#include <SDL.h>

#include "VertexArrayObject.h"
#include "RenderObject.h"
#include "GL_Texture2D.h"

namespace PiE {

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
		std::map<std::string, RenderContext> parseMtl(const char * fileName);
		// parse an obj file
		// * flags: the flags to initialize the vertex array objects with
		std::vector<RenderObject> parseObj(const char * fileName, VertexFormat format);
	}

}

#endif
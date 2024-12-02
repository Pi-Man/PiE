#ifndef _TESSELLATOR_H
#define _TESSELLATOR_H

#include <vector>

#include <glew.h>
#include <gl/GLU.h>

#include "RenderObject.h"

namespace PiE {

	struct Tessellator;

	void default_beginCB(GLenum type, Tessellator * polygon_data);
	void default_vertexCB(void * vertex_data, Tessellator * polygon_data);
	void default_endCB(Tessellator * polygon_data);
	void default_errorCB(GLenum error, Tessellator * polygon_data);

	struct Tessellator {

		void(*beginCB)(GLenum, Tessellator *) = default_beginCB;
		void(*vertexCB)(void *, Tessellator *) = default_vertexCB;
		void(*endCB)(Tessellator *) = default_endCB;
		void(*errorCB)(GLenum, Tessellator *) = default_errorCB;

		GLUtesselator * tessellator = nullptr;

		RenderObject & renderObject;

		GLenum mode = 0;

		Tessellator(RenderObject & renderObject) : renderObject(renderObject) {};

		void begin();

		void addContour(std::vector<std::array<GLdouble, 3>> & pos, std::vector<Vertex> & data);

		void end();
	};

}

#endif
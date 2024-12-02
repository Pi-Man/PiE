#ifndef _UNIFORM_H
#define _UNIFORM_H

#include <string>

#include <glew.h>
#include <SDL.h>

#include "ShaderTypes.h"

namespace PiE {

	struct Uniform {
		GLint location;
		union {
			void * v;
			GLfloat * f;
			GLint * i;
			GLdouble * d;
		} data;
		Shader_Type type;
		GLsizei count;

		void init(std::string name, GLuint shaderID, Shader_Type type, int count = 1);
		void destroy();
		void apply();
	};
}

#endif
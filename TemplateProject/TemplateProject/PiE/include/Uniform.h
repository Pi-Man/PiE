#ifndef _UNIFORM
#define _UNIFORM

#include <string>

#include <glew.h>
#include <SDL.h>

#include "ShaderTypes.h"

struct Uniform {
	GLint location;
	union {
		void* v;
		GLfloat* f;
		GLint* i;
		GLdouble* d;
	} data;
	ShaderTypes::Shader_Type type;
	GLsizei count;

	void init(std::string name, GLuint shaderID, ShaderTypes::Shader_Type type, int count = 1);
	void destroy();
	void apply();
};

#endif
#ifndef _UNIFORM
#define _UNIFORM

#include <string>

#include <glew.h>
#include <SDL.h>

enum Uniform_Type {
	VEC1I,
	VEC2I,
	VEC3I,
	VEC4I,
	VEC1,
	VEC2,
	VEC3,
	VEC4,
	VEC1D,
	VEC2D,
	VEC3D,
	VEC4D,
	MAT4,
	MAT4D
};

struct Uniform {
	GLint location;
	union {
		void* v;
		GLfloat* f;
		GLint* i;
		GLdouble* d;
	} data;
	Uniform_Type type;

	void init(std::string name, GLuint shaderID, Uniform_Type type);
	void destroy();
	void apply();
};

#endif
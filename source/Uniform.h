#ifndef _UNIFORM
#define _UNIFORM

#include <string>

#include <glew.h>
#include <SDL.h>

enum class Uniform_Type {
	INT,
	VEC2I,
	VEC3I,
	VEC4I,
	FLOAT,
	VEC2,
	VEC3,
	VEC4,
	DOUBLE,
	VEC2D,
	VEC3D,
	VEC4D,
	MAT2,
	MAT3,
	MAT4,
	MAT2X3,
	MAT3X2,
	MAT2X4,
	MAT4X2,
	MAT3X4,
	MAT4X3,
	MAT2D,
	MAT3D,
	MAT4D,
	MAT2X3D,
	MAT3X2D,
	MAT2X4D,
	MAT4X2D,
	MAT3X4D,
	MAT4X3D
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
	GLsizei count;

	void init(std::string name, GLuint shaderID, Uniform_Type type, int count = 1);
	void destroy();
	void apply();
};

#endif
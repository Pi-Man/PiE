#ifndef _UNIFORM
#define _UNIFORM

#include <string>

#include <glew.h>
#include <SDL.h>

enum Uniform_Type {
	I,
	VEC2I,
	VEC3I,
	VEC4I,
	F,
	VEC2,
	VEC3,
	VEC4,
	MAT4
};

struct Uniform {
	GLint location;
	void* data;
	Uniform_Type type;

	void init(std::string name, GLuint shaderID, Uniform_Type type);
	template<typename T>
	T* cast();
	void destroy();
	void apply();
};

#endif
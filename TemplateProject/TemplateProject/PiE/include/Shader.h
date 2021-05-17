#ifndef _SHADER
#define _SHADER

#include <cstdio>
#include <iostream>

#include <glew.h>
#include <SDL.h>

#include "Macros.h"

#define GL_SERROR(a, shaderID, shader) GL_ERROR(a); {GLint success; glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success); if (!success) {char log[10000]; glGetShaderInfoLog(shaderID, 10000, nullptr, log); std::cout << "Error Compiling Shader:\n" << log << "\n" << shader << std::endl; return success;}}

class Shader {

	GLint vertID;
	GLint geomID;
	GLint fragID;

public:

	// the GL handle of the shader
	GLint ID;

	Shader();

	// build the shader with the given shader files
	// * vertex shader and fragment shader are required
	// * geometry shader is optional
	int buildShader(const char *vertFile, const char *fragFile, const char* geomFile = nullptr);

	~Shader();
};


#endif
#ifndef _SHADER_H
#define _SHADER_H

#include <cstdio>
#include <iostream>
#include <vector>

#include <glew.h>
#include <SDL.h>

#include "Macros.h"
#include "Uniform.h"

#define GL_SERROR(a, shaderID, shader) GL_ERROR(a); {GLint success; glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success); if (!success) {char log[10000]; glGetShaderInfoLog(shaderID, 10000, nullptr, log); std::cout << "Error Compiling Shader:\n" << log << "\n" << shader << std::endl; return success;}}

namespace PiE {

	struct Shader {

		GLint vertID;
		GLint geomID;
		GLint fragID;

		// the GL handle of the shader
		GLint ID = -1;
		std::vector<Uniform *> uniforms;

		Shader();

		// build the shader with the given shader files
		// * vertex shader and fragment shader are required
		// * geometry shader is optional
		int buildShader(const char * vertFile, const char * fragFile, const char * geomFile = nullptr);

		~Shader();
	};
}

#endif
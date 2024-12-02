#ifndef _GL_TEXTURE_2D_H
#define _GL_TEXTURE_2D_H

#include <memory>
#include <vector>

#include <SDL_image.h>
#include <glew.h>

#include "Macros.h"

namespace PiE {

// a texture that gets sent to the GPU
	struct GL_Texture2D {

		unsigned int * data = nullptr;

		// the width and height of the texture
		size_t width = 0, height = 0;
		// the gl handle of the texture
		GLuint ID = -1;

		GL_Texture2D(const GL_Texture2D & other);
		GL_Texture2D(GL_Texture2D && other);

		GL_Texture2D & operator=(const GL_Texture2D & other);
		GL_Texture2D & operator=(GL_Texture2D && other);

		// creates a texture from an image file
		GL_Texture2D(const char * file, GLint filter = GL_NEAREST);

		// creates a texture with the given size
		GL_Texture2D(size_t width, size_t height, GLint filter = GL_NEAREST);

		~GL_Texture2D();

		// binds the data to the GPU
		void bindData();

		// generates automatic mipmaps of the texture 
		void generateMipMaps();

		// bind the texture to the current GL state to be used for rendering
		void use();

		std::vector<std::reference_wrapper<unsigned int>> operator[](int index);

	};

	bool operator==(const GL_Texture2D & left, const GL_Texture2D & right);
	bool operator!=(const GL_Texture2D & left, const GL_Texture2D & right);

}

#endif
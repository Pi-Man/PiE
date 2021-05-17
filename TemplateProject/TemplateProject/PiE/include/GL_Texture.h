#ifndef _GL_TEXTURE2D
#define _GL_TEXTURE2D

#include <memory>
#include <vector>

#include <SDL_image.h>
#include <glew.h>

#include "Macros.h"

// a texture that gets sent to the GPU
class GL_Texture2D {

	unsigned int *data = nullptr;

public:
	// the width and height of the texture
	size_t width = 0, height = 0;
	// the gl handle of the texture
	GLuint ID = -1;

	// creates a texture from an image file
	GL_Texture2D(const char * file, GLint filter = GL_NEAREST);
	// creates a texture with the given size
	GL_Texture2D(size_t width, size_t height, GLint filter = GL_NEAREST);
	~GL_Texture2D();

	// get the pixels as a single array
	// * use the array operator to access pixels with x and y coordinates
	unsigned int *getData() const;
	// binds the data to the GPU
	void bindData();
	// generates automatic mipmaps of the texture 
	void generateMipMaps();
	// bind the texture to the current GL state to be used for rendering
	void use();

	std::vector<std::reference_wrapper<unsigned int>> operator[](int index);

};

#endif
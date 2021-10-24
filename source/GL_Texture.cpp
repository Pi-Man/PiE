#include "GL_Texture.h"

GL_Texture2D::GL_Texture2D(const char * file, GLint filter) {
	SDL_Surface *s = IMG_Load(file);
	if (s == nullptr) {
		printf("ERROR: could not load Image: %s\n ERROR: %s", file, IMG_GetError());
		return;
	}
	data = (unsigned int*)malloc(sizeof(unsigned int) * s->w * s->h);
	for (int i = 0; i < s->w * s->h; i++) {
		int padding = i/s->w * (s->pitch - s->w * s->format->BytesPerPixel);// i / s->w * sizeof(s->format->padding) / 2;
		data[i] = COLOR_A(
			s->format->Rloss ? 0xff : ((unsigned char *)(s->pixels))[padding + i * s->format->BytesPerPixel + s->format->Rshift / 8],
			s->format->Gloss ? 0xff : ((unsigned char *)(s->pixels))[padding + i * s->format->BytesPerPixel + s->format->Gshift / 8],
			s->format->Bloss ? 0xff : ((unsigned char *)(s->pixels))[padding + i * s->format->BytesPerPixel + s->format->Bshift / 8],
			s->format->Aloss ? 0xff : ((unsigned char *)(s->pixels))[padding + i * s->format->BytesPerPixel + s->format->Ashift / 8]
		);
	}
	width = s->w;
	height = s->h;
	SDL_FreeSurface(s);

	glCreateTextures(GL_TEXTURE_2D, 1, &ID);

	glBindTexture(GL_TEXTURE_2D, ID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

	glBindTexture(GL_TEXTURE_2D, 0);
}

GL_Texture2D::GL_Texture2D(size_t width, size_t height, GLint filter) : width(width), height(height) {

	data = (unsigned int*)malloc(sizeof(unsigned int) * width * height);
	memset(data, 0, sizeof(unsigned int) * width * height);

	glCreateTextures(GL_TEXTURE_2D, 1, &ID);

	glBindTexture(GL_TEXTURE_2D, ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

	glBindTexture(GL_TEXTURE_2D, 0);
}

GL_Texture2D::~GL_Texture2D() {
	free(data);
	glDeleteTextures(1, &ID);
}

unsigned int * GL_Texture2D::getData() const {
	return data;
}

void GL_Texture2D::bindData() {
	glBindTexture(GL_TEXTURE_2D, ID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void GL_Texture2D::generateMipMaps() {
	glBindTexture(GL_TEXTURE_2D, ID);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void GL_Texture2D::use() {
	glBindTexture(GL_TEXTURE_2D, ID);
}

std::vector<std::reference_wrapper<unsigned int>> GL_Texture2D::operator[](int index) {
	std::vector<std::reference_wrapper<unsigned int>> out;
	for (int y = 0; y < height; y++) {
		out.push_back(data[y * width + index]);
	}
	return out;
}

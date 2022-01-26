#pragma once

#include <map>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <RenderObject.h>
#include <Vec.h>

struct Glyph {
	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;
	Vec2f glyphAdvance;
};

struct Font {

	struct RenderInfo {
		FT_Face font;
		std::array<GLdouble, 3>vertex;
		size_t glyphID;
		int line;
		float width;
		float widthMax;
		char character;
	};

	const static FT_Library fontManager;

	const std::string fontName;
	const FT_Face font;

	Font(std::string &fontName);

	std::map<FT_UInt, Glyph> glyphMap{};

	RenderObject buildRenderObject(std::string text);
	RenderObject buildRenderObjectWithAttributes(std::string text, VAO_Flags flags, std::function<std::vector<GLfloat>(const Font::RenderInfo &)> attributeCallback);

	void cacheAllGlyphs();
};


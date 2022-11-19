#pragma once

#include <map>
#include <vector>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "RenderObject.h"
#include "Vec.h"

struct Glyph {
	std::vector<GLdouble> vertices;
	std::vector<GLuint> indices;
	Vec2d glyphAdvance;
};

struct Font {

	struct RenderInfo {
		FT_Face font;
		std::array<GLdouble, 3> vertex;
		std::array<GLdouble, 2> uv;
		size_t glyphID;
		int line;
		size_t lines;
		GLdouble width;
		GLdouble widthMax;
		GLdouble top;
		GLdouble bottom;
		char character;
	};

	const static FT_Library fontManager;
	const static VertexFormat defaultFormat;

	const std::string fontName;
	const FT_Face font;

	Font(std::string & fontName);
	Font(std::string && fontName);

	std::map<FT_UInt, Glyph> glyphMap{};

	RenderObject buildRenderObject(std::string text);
	RenderObject buildRenderObjectWithAttributes(std::string text, VertexFormat format, std::function<void(Vertex &, const Font::RenderInfo &)> attributeCallback);

	void cacheAllGlyphs();
};


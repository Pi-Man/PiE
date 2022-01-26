#include "FontRenderer.h"
#include "Tessellator.h"

FT_Library getFontManager() {
	FT_Library fontManager;
	FT_Error error;
	if (error = FT_Init_FreeType(&fontManager)) {
		puts("Could not create Font Manager");
		return nullptr;
	}
	return fontManager;
}

FT_Face getFontFace(std::string & fontName) {
	FT_Face face;
	FT_Error error;
	if (error = FT_New_Face(Font::fontManager, fontName.c_str(), 0, &face)) {
		puts("Could not create Font");
		return nullptr;
	}
	return face;
}

const FT_Library Font::fontManager = getFontManager();

Font::Font(std::string & fontName) : fontName(fontName), font(getFontFace(fontName)) {}
Font::Font(std::string && fontName) : fontName(fontName), font(getFontFace(fontName)) {}

Glyph loadGlyph(FT_Face font, FT_UInt glyphID) {

	Glyph glyph;

	if (FT_Load_Glyph(font, glyphID, FT_LOAD_NO_SCALE)) {
		puts("ERROR: Could not load glyph");
		return glyph;
	}

	if (font->glyph->format != FT_GLYPH_FORMAT_OUTLINE) {
		puts("WARNING: non-outline fomat");
		return glyph;
	}

	Vec2f ninf{ {-INFINITY, -INFINITY} };
	Vec2f start = ninf, control = ninf, end = ninf;

	FT_Outline outline = font->glyph->outline;

	std::vector<std::vector<std::array<Vec2f, 3>>> contours;

	int contour = -1;
	int dataIndex = 0;
	for (int i = 0; i < outline.n_points; i++) {

		if (i == 0 || i == outline.contours[contour] + 1) {
			contours.push_back(std::vector<std::array<Vec2f, 3>>{});
			contour++;
			//printf("%d\n", dataIndex);
		}

		//printf("(%d, %d): %s\n", outline.points[i].x, outline.points[i].y, outline.tags[i] & FT_CURVE_TAG_ON ? "start/end" : "control");

		if (outline.tags[i] & FT_CURVE_TAG_ON) {
			size_t next = i == outline.contours[contour] ?
				contour == 0 ?
				0 :
				outline.contours[contour - 1] + 1 :
				i + 1;
			if (outline.tags[next] & FT_CURVE_TAG_ON) {
				start[0] = outline.points[i].x / 64.0f;
				start[1] = outline.points[i].y / 64.0f;
				end[0] = outline.points[next].x / 64.0f;
				end[1] = outline.points[next].y / 64.0f;
				control = (start + end) / 2.0f;
			}
		}
		else /*if (outline.tags[i] & FT_CURVE_TAG_CONIC)*/ {
			size_t next = i == outline.contours[contour] ?
				contour == 0 ?
				0 :
				outline.contours[contour - 1] + 1 :
				i + 1;
			size_t prev = i == 0 || i == outline.contours[contour - 1] + 1 ?
				outline.contours[contour] :
				i - 1;

			if (outline.tags[prev] & FT_CURVE_TAG_ON) {
				start[0] = outline.points[prev].x / 64.0f;
				start[1] = outline.points[prev].y / 64.0f;
			}
			else /*if (outline.tags[prev] & FT_CURVE_TAG_CONIC)*/ {
				start[0] = (outline.points[prev].x + outline.points[i].x) * 0.5f / 64.0f;
				start[1] = (outline.points[prev].y + outline.points[i].y) * 0.5f / 64.0f;
			}

			if (outline.tags[next] & FT_CURVE_TAG_ON) {
				end[0] = outline.points[next].x / 64.0f;
				end[1] = outline.points[next].y / 64.0f;
			}
			else /*if (outline.tags[next] & FT_CURVE_TAG_CONIC)*/ {
				end[0] = (outline.points[next].x + outline.points[i].x) * 0.5f / 64.0f;
				end[1] = (outline.points[next].y + outline.points[i].y) * 0.5f / 64.0f;
			}

			control[0] = outline.points[i].x / 64.0f;
			control[1] = outline.points[i].y / 64.0f;
		}

		if (end != ninf) {
			contours[contour].push_back({ start, control, end });
			//printf("vec3(%f, %f),\nvec3(%f, %f),\nvec3(%f, %f),\n", start[0], start[1], control[0], control[1], end[0], end[1]);
			start = control = end = ninf;
			//dataIndex += 3;
		}


	}

	std::vector<std::vector<std::array<GLdouble, 3>>> polygon;

	RenderObject renderObject;
	renderObject.VAO = VertexArrayObject(VAO_UVS);
	for (const std::vector<std::array<Vec2f, 3>> &contour : contours) {
		std::vector<std::array<GLdouble, 3>> points;
		for (const std::array<Vec2f, 3> &curve : contour) {
			Vec2f start, control, end;
			start = curve[0];
			control = curve[1];
			end = curve[2];
			if (curve[1] != ninf) {
				renderObject.VAO.addTriangleUVs(std::array<float, 15>{
					start[0], start[1], 0.0f,
						0.0f, 0.0f,
						control[0], control[1], 0.0f,
						0.5f, 0.0f,
						end[0], end[1], 0.0f,
						1.0f, 1.0f,
				});
			}
			float cross = (control - start) ^ (end - control);
			points.push_back({ start[0], start[1], 0.0 });
			if (curve[1] != ninf && cross > 0) {
				points.push_back({ control[0], control[1], 0.0 });
			}
		}
		polygon.push_back(points);
	}

	Tessellator tess(renderObject);
	tess.begin();
	for (std::vector<std::array<GLdouble, 3>> &points : polygon) {
		tess.addContour(points);
	}
	tess.end();

	glyph.vertices = renderObject.VAO.buffer;
	glyph.indices = renderObject.VAO.indicies;

	glyph.glyphAdvance = { {font->glyph->advance.x / 64.0f, font->glyph->advance.y / 64.0f} };

	return glyph;
}

RenderObject Font::buildRenderObject(std::string text) {

	RenderObject RO{};
	RO.VAO = VertexArrayObject(VAO_UVS);
	RO.renderContext.cullEnable = false;

	Vec2f pen{ { 0, 0 } };

	for (const char & c : text) {

		if (c == '\n') {
			pen[0] = 0;
			pen[1] -= font->height / 64.0f;
			continue;
		}

		Glyph glyph;
		FT_UInt glyphID = FT_Get_Char_Index(font, c);

		auto it = glyphMap.find(glyphID);
		if (it == glyphMap.end()) {
			glyph = loadGlyph(font, glyphID);
			glyphMap[glyphID] = glyph;
		}
		else {
			glyph = it->second;
		}

		for (size_t i = 0; i < glyph.vertices.size(); i += 5) {
			RO.VAO.buffer.push_back(glyph.vertices[i + 0] + pen[0]);
			RO.VAO.buffer.push_back(glyph.vertices[i + 1] + pen[1]);
			RO.VAO.buffer.push_back(glyph.vertices[i + 2]);
			RO.VAO.buffer.push_back(glyph.vertices[i + 3]);
			RO.VAO.buffer.push_back(glyph.vertices[i + 4]);
		}

		size_t lastIndex = RO.VAO.indicies.size();
		for (const GLuint & index : glyph.indices) {
			RO.VAO.indicies.push_back((GLuint)lastIndex + index);
		}

		pen += glyph.glyphAdvance;
	}

	return RO;
}

RenderObject Font::buildRenderObjectWithAttributes(std::string text, VAO_Flags flags, std::function<void(std::vector<GLfloat> & out, const Font::RenderInfo &)> attributeCallback) {

	RenderObject RO{};
	RO.VAO = VertexArrayObject(flags | VAO_UVS);
	RO.renderContext.cullEnable = false;

	Vec2f pen{ { 0, 0 } };

	float widthMax = 0;
	float width = 0;
	std::vector<float> widths;
	for (const char & c : text) {

		if (c == '\n') {
			if (width > widthMax) widthMax = width;
			widths.push_back(width);
			width = 0;
			continue;
		}

		Glyph glyph;
		FT_UInt glyphID = FT_Get_Char_Index(font, c);

		auto it = glyphMap.find(glyphID);
		if (it == glyphMap.end()) {
			glyph = loadGlyph(font, glyphID);
			glyphMap[glyphID] = glyph;
		}
		else {
			glyph = it->second;
		}

		RO.VAO.buffer.reserve(RO.VAO.buffer.size() + RO.VAO.stride * glyph.vertices.size());
		RO.VAO.indicies.reserve(RO.VAO.indicies.size() + glyph.indices.size());

		width += glyph.glyphAdvance[0];
	}
	if (width > widthMax) widthMax = width;
	widths.push_back(width);

	int line = 0;
	for (const char & c : text) {

		if (c == '\n') {
			pen[0] = 0;
			pen[1] -= font->height / 64.0f;
			line++;
			continue;
		}

		FT_UInt glyphID = FT_Get_Char_Index(font, c);
		Glyph glyph = glyphMap[glyphID];

		std::vector<GLfloat> vertexData(RO.VAO.stride - 2, 0);
		for (size_t i = 0; i < glyph.vertices.size(); i += 5) {
			attributeCallback(vertexData, {
				font,
				{glyph.vertices[i] + pen[0], glyph.vertices[i + 1] + pen[1], glyph.vertices[2] },
				glyphID,
				line,
				(int)widths.size(),
				widths[line],
				widthMax,
				font->ascender / 64.0f,
				-font->height / 64.0f * (widths.size() - 1),
				c
				});

			if (vertexData.size() != (RO.VAO.stride - 2)) {
				printf("Vertex Data is not correct size!  Expected %d, got %zd", (RO.VAO.stride - 2), vertexData.size());
				return RO;
			}

			int j = 0;
			RO.VAO.buffer.push_back(vertexData[j++]);
			RO.VAO.buffer.push_back(vertexData[j++]);
			RO.VAO.buffer.push_back(vertexData[j++]);

			if (RO.VAO.flags & VAO_NORMALS) {
				RO.VAO.buffer.push_back(vertexData[j++]);
				RO.VAO.buffer.push_back(vertexData[j++]);
				RO.VAO.buffer.push_back(vertexData[j++]);
			}

			RO.VAO.buffer.push_back(glyph.vertices[i + 3]);
			RO.VAO.buffer.push_back(glyph.vertices[i + 4]);

		}

		size_t lastIndex = RO.VAO.indicies.size();
		for (const GLuint & index : glyph.indices) {
			RO.VAO.indicies.push_back((GLuint)lastIndex + index);
		}

		pen += glyph.glyphAdvance;
	}

	return RO;
}

void Font::cacheAllGlyphs() {
}

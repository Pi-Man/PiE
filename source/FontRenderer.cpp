#include "FontRenderer.h"
#include "Tessellator.h"

namespace PiE {

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
	//const VertexFormat Font::defaultFormat{ POSITION_D, UV2_D };

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

		Vec2d ninf{ {-INFINITY, -INFINITY} };
		Vec2d start = ninf, control = ninf, end = ninf;

		FT_Outline outline = font->glyph->outline;

		std::vector<std::vector<std::array<Vec2d, 3>>> contours;

		int contour = -1;
		int dataIndex = 0;
		for (int i = 0; i < outline.n_points; i++) {

			if (i == 0 || i == outline.contours[contour] + 1) {
				contours.push_back(std::vector<std::array<Vec2d, 3>>{});
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
					start[0] = outline.points[i].x / 64.0;
					start[1] = outline.points[i].y / 64.0;
					end[0] = outline.points[next].x / 64.0;
					end[1] = outline.points[next].y / 64.0;
					control = (start + end) / 2.0;
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
					start[0] = outline.points[prev].x / 64.0;
					start[1] = outline.points[prev].y / 64.0;
				}
				else /*if (outline.tags[prev] & FT_CURVE_TAG_CONIC)*/ {
					start[0] = (outline.points[prev].x + outline.points[i].x) * 0.5 / 64.0;
					start[1] = (outline.points[prev].y + outline.points[i].y) * 0.5 / 64.0;
				}

				if (outline.tags[next] & FT_CURVE_TAG_ON) {
					end[0] = outline.points[next].x / 64.0;
					end[1] = outline.points[next].y / 64.0;
				}
				else /*if (outline.tags[next] & FT_CURVE_TAG_CONIC)*/ {
					end[0] = (outline.points[next].x + outline.points[i].x) * 0.5f / 64.0;
					end[1] = (outline.points[next].y + outline.points[i].y) * 0.5f / 64.0;
				}

				control[0] = outline.points[i].x / 64.0;
				control[1] = outline.points[i].y / 64.0;
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
		renderObject.VAO = VertexArrayObject(Font::defaultFormat);
		for (const std::vector<std::array<Vec2d, 3>> &contour : contours) {
			std::vector<std::array<GLdouble, 3>> points;
			for (const std::array<Vec2d, 3> &curve : contour) {
				Vec2d start, control, end;
				start = curve[0];
				control = curve[1];
				end = curve[2];
				if (curve[1] != ninf) {
					renderObject.VAO.addTriangle({
						Font::defaultFormat,
							{ start[0], start[1], 0.0 },
							{ 0.0, 0.0},
						}, {
						Font::defaultFormat,
							{ control[0], control[1], 0.0},
							{ 0.5, 0.0},
						}, {
						Font::defaultFormat,
							{ end[0], end[1], 0.0},
							{ 1.0, 1.0},
						}
						);
				}
				double cross = (control - start) ^ (end - control);
				points.push_back({ start[0], start[1], 0.0 });
				if (curve[1] != ninf && cross > 0) {
					points.push_back({ control[0], control[1], 0.0 });
				}
			}
			polygon.push_back(points);
		}

		Tessellator tess(renderObject);
		tess.begin();
		std::vector<std::vector<Vertex>> vertices;
		for (std::vector<std::array<GLdouble, 3>> &points : polygon) {
			vertices.push_back(std::vector<Vertex>());
			vertices.back().reserve(points.size());
			for (std::array<GLdouble, 3> &point : points) {
				vertices.back().push_back({ Font::defaultFormat,
					{ (float)point[0], (float)point[1], (float)point[2] },
					{ 0.5f, 0.5f }
					});
			}
			tess.addContour(points, vertices.back());
		}
		tess.end();

		glyph.vertices = { (float *)renderObject.VAO.buffer.data(), (float *)(renderObject.VAO.buffer.data() + renderObject.VAO.buffer.size()) };
		glyph.indices = renderObject.VAO.indicies;

		glyph.glyphAdvance = { {font->glyph->advance.x / 64.0f, font->glyph->advance.y / 64.0f} };

		return glyph;
	}

	RenderObject Font::buildRenderObject(std::string text) {

		RenderObject RO{};
		RO.VAO = VertexArrayObject(Font::defaultFormat);
		RO.renderContext.cullEnable = false;

		Vec2f pen{ { 0, 0 } };

		for (const char & c : text) {

			if (c == '\n') {
				pen[0] = 0;
				pen[1] -= font->height / 64.0;
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

			for (size_t i = 0; i < glyph.vertices.size(); i += 5*3) {
				RO.VAO.addTriangle({
					Font::defaultFormat,
						{glyph.vertices[i + 0] + pen[0], glyph.vertices[i + 1] + pen[1], glyph.vertices[i + 2]},
						{glyph.vertices[i + 3], glyph.vertices[i + 4]}
					}, {
					Font::defaultFormat,
						{glyph.vertices[i + 5] + pen[0], glyph.vertices[i + 6] + pen[1], glyph.vertices[i + 7]},
						{glyph.vertices[i + 8], glyph.vertices[i + 9]}
					}, {
					Font::defaultFormat,
						{glyph.vertices[i + 10] + pen[0], glyph.vertices[i + 11] + pen[1], glyph.vertices[i + 12]},
						{glyph.vertices[i + 13], glyph.vertices[i + 14]}
					});
			}

			size_t lastIndex = RO.VAO.indicies.size();
			for (const size_t & index : glyph.indices) {
				RO.VAO.indicies.push_back((GLuint)(lastIndex + index));
			}

			pen += glyph.glyphAdvance;
		}

		return RO;
	}

	RenderObject Font::buildRenderObjectWithAttributes(std::string text, VertexFormat format, std::function<void(Vertex & out, const Font::RenderInfo &)> attributeCallback) {

		RenderObject RO{};
		RO.VAO = VertexArrayObject(format);
		RO.renderContext.cullEnable = false;

		Vec2f pen{ { 0, 0 } };

		GLdouble widthMax = 0;
		GLdouble width = 0;
		std::vector<GLdouble> widths;
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

			RO.VAO.buffer.reserve(RO.VAO.buffer.size() + RO.VAO.format.stride * glyph.vertices.size() / 5);
			RO.VAO.indicies.reserve(RO.VAO.indicies.size() + glyph.indices.size());

			width += glyph.glyphAdvance[0];
		}
		if (width > widthMax) widthMax = width;
		widths.push_back(width);

		int line = 0;
		for (const char & c : text) {

			if (c == '\n') {
				pen[0] = 0;
				pen[1] -= font->height / 64.0;
				line++;
				continue;
			}

			FT_UInt glyphID = FT_Get_Char_Index(font, c);
			Glyph glyph = glyphMap[glyphID];

			std::vector<Vertex> vertices;
			vertices.reserve(glyph.vertices.size() / 5);
			for (size_t i = 0; i < glyph.vertices.size(); i += 5) {
				Vertex v(Font::defaultFormat);
				attributeCallback(v, {
					font,
					{ glyph.vertices[i] + pen[0], glyph.vertices[i + 1] + pen[1], glyph.vertices[i + 2] },
					{ glyph.vertices[i + 3], glyph.vertices[i + 4] },
					glyphID,
					line,
					widths.size(),
					widths[line],
					widthMax,
					(GLdouble)font->ascender / 64.0,
					(GLdouble)-font->height / 64.0 * (widths.size() - 1),
					c
					});

				vertices.push_back(v);

			}

			RO.VAO.addMesh(vertices, glyph.indices);

			pen += glyph.glyphAdvance;
		}

		return RO;
	}

	void Font::cacheAllGlyphs() {}

}
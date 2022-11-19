#include "Tessellator.h"

Vertex * triangleStripPoints[2];
bool triangleStripPointsValid[2];
bool triangleStripSide = false;

void default_beginCB(GLenum type, Tessellator * polygon_data) {
	Tessellator *tessellator = (Tessellator*)polygon_data;
	tessellator->mode = type;
	triangleStripSide = triangleStripPointsValid[0] = triangleStripPointsValid[1] = false;
}

void default_vertexCB(void * vertex_data, Tessellator * polygon_data) {
	Vertex &vertex = *(Vertex*)vertex_data;
	Tessellator *tessellator = (Tessellator*)polygon_data;

	if (!triangleStripPointsValid[0]) {
		triangleStripPoints[0] = (Vertex*)vertex_data;
		triangleStripPointsValid[0] = true;
		return;
	}
	if (!triangleStripPointsValid[1]) {
		triangleStripPoints[1] = (Vertex*)vertex_data;
		triangleStripPointsValid[1] = true;
		return;
	}

	tessellator->renderObject.VAO.addTriangle(*triangleStripPoints[triangleStripSide], *triangleStripPoints[1 - triangleStripSide], vertex);

	if (tessellator->mode == GL_TRIANGLE_STRIP) {
		triangleStripPoints[0] = triangleStripPoints[1];
		triangleStripPoints[1] = (Vertex*)vertex_data;
		triangleStripSide ^= true;
	}
	else if (tessellator->mode == GL_TRIANGLES) {
		triangleStripPointsValid[0] = triangleStripPointsValid[1] = false;
	}
	else if (tessellator->mode == GL_TRIANGLE_FAN) {
		triangleStripPoints[1] = (Vertex*)vertex_data;
	}

}

void default_endCB(Tessellator * polygon_data) {
	Tessellator *tessellator = (Tessellator*)polygon_data;
}

void default_errorCB(GLenum error, Tessellator * polygon_data) {
	Tessellator *tessellator = (Tessellator*)polygon_data;
	printf("Error during Tessellation: %s", (const unsigned char *)gluErrorString(error));
}

void Tessellator::begin() {
	tessellator = gluNewTess();
	gluTessCallback(tessellator, GLU_TESS_BEGIN_DATA, (void(*)())beginCB);
	gluTessCallback(tessellator, GLU_TESS_VERTEX_DATA, (void(*)())vertexCB);
	gluTessCallback(tessellator, GLU_TESS_END_DATA, (void(*)())endCB);
	gluTessCallback(tessellator, GLU_TESS_ERROR_DATA, (void(*)())errorCB);
	gluTessBeginPolygon(tessellator, this);
}

void Tessellator::addContour(std::vector<std::array<GLdouble, 3>> &pos, std::vector<Vertex> &data) {
	gluTessBeginContour(tessellator);
	for (size_t i = 0; i < pos.size(); i++) {
		gluTessVertex(tessellator, pos[i].data(), &data[i]);
	}
	gluTessEndContour(tessellator);
}

void Tessellator::end() {
	gluTessEndPolygon(tessellator);
	gluDeleteTess(tessellator);
	tessellator = nullptr;
}

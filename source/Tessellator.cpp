#include "Tessellator.h"

GLdouble triangleStripPoints[2][3];
bool triangleStripPointsValid[2];
bool triangleStripSide = false;

void default_beginCB(GLenum type, Tessellator * polygon_data) {
	Tessellator *tessellator = (Tessellator*)polygon_data;
	tessellator->mode = type;
	triangleStripSide = triangleStripPointsValid[0] = triangleStripPointsValid[1] = false;
}

void default_vertexCB(void * vertex_data, Tessellator * polygon_data) {
	GLdouble * vertex = (GLdouble*)vertex_data;
	Tessellator *tessellator = (Tessellator*)polygon_data;

	if (!triangleStripPointsValid[0]) {
		memcpy(triangleStripPoints[0], vertex, sizeof(GLdouble) * 3);
		triangleStripPointsValid[0] = true;
		return;
	}
	if (!triangleStripPointsValid[1]) {
		memcpy(triangleStripPoints[1], vertex, sizeof(GLdouble) * 3);
		triangleStripPointsValid[1] = true;
		return;
	}

	tessellator->renderObject.VAO.addTriangleUVs(std::array<GLfloat, 15>{
		(GLfloat)triangleStripPoints[triangleStripSide][0], (GLfloat)triangleStripPoints[triangleStripSide][1], (GLfloat)triangleStripPoints[triangleStripSide][2],
			0.5f, 0.5f,
			(GLfloat)triangleStripPoints[1 - triangleStripSide][0], (GLfloat)triangleStripPoints[1 - triangleStripSide][1], (GLfloat)triangleStripPoints[1 - triangleStripSide][2],
			0.5f, 0.5f,
			(GLfloat)vertex[0], (GLfloat)vertex[1], (GLfloat)vertex[2],
			0.5f, 0.5f
	});

	if (tessellator->mode == GL_TRIANGLE_STRIP) {
		memcpy(triangleStripPoints[0], triangleStripPoints[1], sizeof(GLdouble) * 3);
		memcpy(triangleStripPoints[1], vertex, sizeof(GLdouble) * 3);
		triangleStripSide ^= true;
	}
	else if (tessellator->mode == GL_TRIANGLES) {
		triangleStripPointsValid[0] = triangleStripPointsValid[1] = false;
	}
	else if (tessellator->mode == GL_TRIANGLE_FAN) {
		memcpy(triangleStripPoints[1], vertex, sizeof(GLdouble) * 3);
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

void Tessellator::addContour(std::vector<std::array<GLdouble, 3>> &data) {
	gluTessBeginContour(tessellator);
	for (size_t i = 0; i < data.size(); i++) {
		gluTessVertex(tessellator, data[i].data(), data[i].data());
	}
	gluTessEndContour(tessellator);
}

void Tessellator::end() {
	gluTessEndPolygon(tessellator);
	gluDeleteTess(tessellator);
	tessellator = nullptr;
}

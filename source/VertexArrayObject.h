#ifndef _VERTEX_ARRAY_OBJECT
#define _VERTEX_ARRAY_OBJECT

#include <glew.h>
#include <vector>
#include <array>

#include "Vec3f.h"

enum VAO_Flags {
	VAO_NORMALS = 1 << 0,   // enable normals
	VAO_UVS     = 1 << 1,   // enable uvs
	VAO_COLOR   = 1 << 2,   // enable color (not yet implemented)
};

// holds the vertex and attribute data to be sent to the GPU to render a single object
class VertexArrayObject {
	GLuint ID = -1;
	GLuint bufferID = -1;
	GLuint elementID = -1;
	GLuint textureID = -1;
	bool gen = false;
	std::vector<GLfloat> buffer;
	std::vector<GLuint> indicies;

	long flags;
	int stride;

	std::vector<GLfloat> calculateNormal(std::vector<GLfloat> && vertexes);

public:
	// creates a vertex array object with the specified attributes (to be or-ed together)
	// * default: vertices only
	VertexArrayObject(long flags = 0);

	// add a triangle
	// * automatically calculates normals
	// * automatically assignes xz coordinates to uv coordinates
	void addTriangle(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, GLfloat x3, GLfloat y3, GLfloat z3);
	// add a triangle
	// * automatically calculates normals
	// * automatically assignes xz coordinates to uv coordinates
	void addTriangle(std::array<GLfloat, 9> pos);
	// add a triangle with the specified uvs
	// * automatically calculates normals
	void addTriangleUVs(std::array<GLfloat, 15> pos);
	// add a triangle with the specified normals
	// * automatically assignes xz coordinates to uv coordinates
	void addTriangleWithNormals(std::array<std::array<GLfloat, 6>, 3> vertices);
	// add a triangle with the specified normals
	// * automatically assignes xz coordinates to uv coordinates
	void addTriangleWithNormals(std::array<GLfloat, 6 * 3> vertices);
	// add a triangle with the specified normals and uvs
	void addTriangelUVsNormals(std::array<GLfloat, 3 * (3 + 3 + 2)> data);

	// add a quadralateral
	// * automatically calculates normals
	// * automatically assignes xz coordinates to uv coordinates
	void addQuad(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, GLfloat x3, GLfloat y3, GLfloat z3, GLfloat x4, GLfloat y4, GLfloat z4);
	// add a quadralateral
	// * automatically calculates normals
	// * automatically assignes xz coordinates to uv coordinates
	void addQuad(std::array<GLfloat, 12> pos);
	// add a quadralateral with the specified normals
	// * automatically assignes xz coordinates to uv coordinates
	void addQuadWithNormals(std::array<GLfloat, 24> data);
	// add a quadralateral with the specified uvs
	// * automatically calculates normals
	void addQuadWithUVs(std::array<GLfloat, 20> data);
	// add a quadralateral with the specified normals and uvs
	void addQuadUVsNormals(std::array<GLfloat, 4 * (3 + 3 + 2)> data);

	// add a cuboid
	// * warning does not support normals or uvs
	void addCuboid(std::array<GLfloat, 24> pos);

	// add a polygon
	// * automatically calculates normals
	// * automatically assignes xz coordinates to uv coordinates
	void addPolygon(std::vector<std::array<float, 3>> points);
	// add a polygon with the specified normals
	// * automatically assignes xz coordinates to uv coordinates
	void addPolygonWithNormals(std::vector<std::array<float, 6>> points);

	// adds all objects of an obj model to the VAO
	// * WARNING: DEPRECATED
	// ** use OBJ::parseObj(const char *fileName, Uint32 flags) instead!
	void addOBJModel(const std::string &&fileName);

	// adds a height mesh to the VAO
	// * x: the min x location
	// * y: the min y location
	// * width: the width of the plane
	// * height: the height of the plane
	// * heightMap: the height map array
	// ** the outside ring of the map is only used to calculate normals and not in setting the height of the plane
	void addHeightMesh(float x, float z, float width, float height, std::vector<std::vector<GLfloat>> heightMap);

	// bind the buffer data to the GPU
	void bindBuffers();

	// DEPRECATED: DO NOT USE
	void copy(VertexArrayObject& other);

	// render the vertex array object
	void render();
};

#endif
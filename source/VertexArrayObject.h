#ifndef _VERTEX_ARRAY_OBJECT
#define _VERTEX_ARRAY_OBJECT

#include <glew.h>
#include <vector>
#include <array>

#include "Vec.h"
#include "VertexAttribute.h"

// holds the vertex and attribute data to be sent to the GPU to render a single object
struct VertexArrayObject {
	GLuint ID = -1;
	GLuint bufferID = -1;
	GLuint elementID = -1;
	bool gen = false;

	static const VertexFormat default_format;
	VertexFormat format;

	std::vector<char> buffer;
	std::vector<GLuint> indicies;

	std::vector<GLfloat> calculateNormal(std::vector<GLfloat> && vertexes);

	// creates a vertex array object with the given vertex format
	// * default: vertices only
	VertexArrayObject(VertexFormat format = default_format);

	void addTriangle(Vertex v1, Vertex v2, Vertex v3);
	void addQuad(Vertex v1, Vertex v2, Vertex v3, Vertex v4);
	void addCuboid(Vertex v1, Vertex v2, Vertex v3, Vertex v4, Vertex v5, Vertex v6, Vertex v7, Vertex v8);
	void addCuboidQuads(Vertex v1, Vertex v2, Vertex v3, Vertex v4, Vertex v5, Vertex v6, Vertex v7, Vertex v8);

	//// add a triangle
	//// * automatically calculates normals
	//// * automatically assignes xz coordinates to uv coordinates
	//void addTriangle(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, GLfloat x3, GLfloat y3, GLfloat z3);
	//// add a triangle
	//// * automatically calculates normals
	//// * automatically assignes xz coordinates to uv coordinates
	//void addTriangle(std::array<GLfloat, 9> pos);
	//// add a triangle
	//// * automatically calculates normals
	//// * automatically assignes xz coordinates to uv coordinates
	//void addTriangle(std::array<std::array<GLfloat, 3>, 3> pos);
	//// add a triangle with the specified uvs
	//// * automatically calculates normals
	//void addTriangleUVs(std::array<GLfloat, 15> pos);
	//// add a triangle with the specified uvs
	//// * automatically calculates normals
	//void addTriangleUVs(std::array<std::array<GLfloat, 5>, 3> pos);
	//// add a triangle with the specified normals
	//// * automatically assignes xz coordinates to uv coordinates
	//void addTriangleWithNormals(std::array<std::array<GLfloat, 6>, 3> vertices);
	//// add a triangle with the specified normals
	//// * automatically assignes xz coordinates to uv coordinates
	//void addTriangleWithNormals(std::array<GLfloat, 6 * 3> vertices);
	//// add a triangle with the specified normals and uvs
	//void addTriangelUVsNormals(std::array<GLfloat, 3 * (3 + 3 + 2)> data);

	//// add a quadralateral
	//// * automatically calculates normals
	//// * automatically assignes xz coordinates to uv coordinates
	//void addQuad(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, GLfloat x3, GLfloat y3, GLfloat z3, GLfloat x4, GLfloat y4, GLfloat z4);
	//// add a quadralateral
	//// * automatically calculates normals
	//// * automatically assignes xz coordinates to uv coordinates
	//void addQuad(std::array<GLfloat, 12> pos);
	//// add a quadralateral with the specified normals
	//// * automatically assignes xz coordinates to uv coordinates
	//void addQuadWithNormals(std::array<GLfloat, 24> data);
	//// add a quadralateral with the specified uvs
	//// * automatically calculates normals
	//void addQuadWithUVs(std::array<GLfloat, 20> data);
	//// add a quadralateral with the specified normals and uvs
	//void addQuadUVsNormals(std::array<GLfloat, 4 * (3 + 3 + 2)> data);

	//// add a cuboid
	//// * warning does not support normals or uvs
	//void addCuboid(std::array<GLfloat, 24> pos);

	//// add a polygon
	//// * automatically calculates normals
	//// * automatically assignes xz coordinates to uv coordinates
	//void addPolygon(std::vector<std::array<float, 3>> points);

	//void addComplexPolygon(const std::vector<std::vector<std::array<float, 3>>> &contours);

	// add a polygon
	// * automatically calculates normals
	// * automatically assignes xz coordinates to uv coordinates
	void addPolygon(std::vector<Vertex> vertices);

	void addMesh(const std::vector<Vertex> & vertices, const std::vector<GLuint> & indices);

	//// add a polygon with the specified normals
	//// * automatically assignes xz coordinates to uv coordinates
	//void addPolygonWithNormals(std::vector<std::array<float, 6>> points);

	//// adds all objects of an obj model to the VAO
	//// * WARNING: DEPRECATED
	//// ** use OBJ::parseObj(const char *fileName, Uint32 flags) instead!
	//void addOBJModel(const std::string &&fileName);

	//// adds a height mesh to the VAO
	//// * x: the min x location
	//// * y: the min y location
	//// * width: the width of the plane
	//// * height: the height of the plane
	//// * heightMap: the height map array
	//// ** the outside ring of the map is only used to calculate normals and not in setting the height of the plane
	//void addHeightMesh(float x, float z, float width, float height, std::vector<std::vector<GLfloat>> heightMap);

	// bind the buffer data to the GPU
	void bindBuffers();

	// DEPRECATED: DO NOT USE
	void copy(VertexArrayObject& other);

	// render the vertex array object
	void render();
};

#endif
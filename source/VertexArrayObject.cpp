#include <set>

#include <SDL.h>

#include "VertexArrayObject.h"

// TODO: major refactoring needed

const VertexAttributeType POSITION_F(Usage::LOCATION, Attribute().make<Attribute_Type::VEC3>({ 0, 0, 0 }));
const VertexAttributeType NORMAL_F(Usage::DIRECTION, Attribute().make<Attribute_Type::VEC3>({ 0, 1, 0 }));
const VertexAttributeType COLOR_F(Usage::OTHER, Attribute().make<Attribute_Type::VEC3>({ 1, 1, 1 }));
const VertexAttributeType UV2_F(Usage::TEXTURE_LOCATION, Attribute().make<Attribute_Type::VEC2>({ 0, 0 }));

const VertexAttributeType POSITION_D(Usage::LOCATION, Attribute().make<Attribute_Type::VEC3D>({ 0, 0, 0 }));
const VertexAttributeType NORMAL_D(Usage::DIRECTION, Attribute().make<Attribute_Type::VEC3D>({ 0, 1, 0 }));
const VertexAttributeType COLOR_D(Usage::OTHER, Attribute().make<Attribute_Type::VEC3D>({ 1, 1, 1 }));
const VertexAttributeType UV2_D(Usage::TEXTURE_LOCATION, Attribute().make<Attribute_Type::VEC2D>({ 0, 0 }));

std::vector<GLfloat> VertexArrayObject::calculateNormal(std::vector<GLfloat> && vertices) {
	float x1, x2, y1, y2, z1, z2,
		x3, y3, z3;
	x1 = vertices[3] - vertices[0];
	y1 = vertices[4] - vertices[1];
	z1 = vertices[5] - vertices[2];
	x2 = vertices[6] - vertices[3];
	y2 = vertices[7] - vertices[4];
	z2 = vertices[8] - vertices[5];

	x3 = y1 * z2 - z1 * y2;
	y3 = z1 * x2 - x1 * z2;
	z3 = x1 * y2 - y1 * x2;

	float magnitude = -sqrtf(x3*x3 + y3 * y3 + z3 * z3);

	x3 /= magnitude;
	y3 /= magnitude;
	z3 /= magnitude;

	return std::vector<GLfloat> {x3, y3, z3};
}

inline std::array<GLfloat, 3> cross(std::array<GLfloat, 3> &&a, std::array<GLfloat, 3> &&b) {
	std::array<GLfloat, 3> out;
	out[0] = a[1] * b[2] - a[2] * b[1];
	out[1] = a[2] * b[0] - a[0] * b[2];
	out[2] = a[0] * b[1] - a[1] * b[0];
	return out;
}

inline std::array<GLfloat, 3> cross(std::array<GLfloat, 3> &a, std::array<GLfloat, 3> &b) {
	std::array<GLfloat, 3> out;
	out[0] = a[1] * b[2] - a[2] * b[1];
	out[1] = a[2] * b[0] - a[0] * b[2];
	out[2] = a[0] * b[1] - a[1] * b[0];
	return out;
}

inline std::array<GLfloat, 3> calculateTCross(std::array<GLfloat, 3> && a, std::array<GLfloat, 3> && b, std::array<GLfloat, 3> && c) {
	std::array<GLfloat, 3> s2 = { c[0] - b[0], c[1] - b[1], c[2] - b[2] };
	std::array<GLfloat, 3> s1 = { b[0] - a[0], b[1] - a[1], b[2] - a[2] };
	return cross(s1, s2);
}

inline std::array<GLfloat, 3> calculateTCross(std::array<GLfloat, 3> & a, std::array<GLfloat, 3> & b, std::array<GLfloat, 3> & c) {
	std::array<GLfloat, 3> s2 = { c[0] - b[0], c[1] - b[1], c[2] - b[2] };
	std::array<GLfloat, 3> s1 = { b[0] - a[0], b[1] - a[1], b[2] - a[2] };
	return cross(s1, s2);
}

inline std::array<GLfloat, 3> normalize(std::array<GLfloat, 3> &&v) {
	GLfloat magnitude = sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	v[0] /= magnitude;
	v[1] /= magnitude;
	v[2] /= magnitude;
	return v;
}

inline std::array<GLfloat, 3> normalize(std::array<GLfloat, 3> &v) {
	GLfloat magnitude = sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	v[0] /= magnitude;
	v[1] /= magnitude;
	v[2] /= magnitude;
	return v;
}

VertexArrayObject::VertexArrayObject(long flags) : flags(flags), stride(3 + !!(flags & VAO_NORMALS) * 3 + !!(flags & VAO_UVS) * 2 + !!(flags & VAO_COLOR) * 3) {}

void VertexArrayObject::addTriangle(std::array<VertexData, 3> points) {
	for (const VertexData & vertexData : points) {
		if (vertexData.format != format) {
			puts("WARNING: mismatched vertex format");
			break;
		}
		size_t i = 0;
		for (const VertexAttributeType & type : format.attributes) {

		}
	}
}

void VertexArrayObject::addTriangle(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, GLfloat x3, GLfloat y3, GLfloat z3) {
	
	int i = (int)buffer.size() / stride;

	bool normals = flags & VAO_NORMALS;
	bool uvs = flags & VAO_UVS;
	std::vector<GLfloat> normal;

	if (normals) normal = calculateNormal({x1, y1, z1, x2, y2, z2, x3, y3, z3});

	buffer.push_back(x1);
	buffer.push_back(y1);
	buffer.push_back(z1);

	if (normals) {
		buffer.push_back(normal[0]);
		buffer.push_back(normal[1]);
		buffer.push_back(normal[2]);
	}

	if (uvs) {
		buffer.push_back(x1);
		buffer.push_back(z1);
	}

	buffer.push_back(x2);
	buffer.push_back(y2);
	buffer.push_back(z2);

	if (normals) {
		buffer.push_back(normal[0]);
		buffer.push_back(normal[1]);
		buffer.push_back(normal[2]);
	}

	if (uvs) {
		buffer.push_back(x2);
		buffer.push_back(z2);
	}

	buffer.push_back(x3);
	buffer.push_back(y3);
	buffer.push_back(z3);

	if (normals) {
		buffer.push_back(normal[0]);
		buffer.push_back(normal[1]);
		buffer.push_back(normal[2]);
	}

	if (uvs) {
		buffer.push_back(x3);
		buffer.push_back(z3);
	}

	indicies.push_back(i++);
	indicies.push_back(i++);
	indicies.push_back(i++);
}

void VertexArrayObject::addTriangle(std::array<GLfloat, 9> pos) {

	int i = (int)buffer.size() / stride;

	std::vector<GLfloat> normal;
	if (flags & VAO_NORMALS) normal = calculateNormal(std::vector<GLfloat>(pos.begin(), pos.end()));

	for (int i = 0; i < 3; i++) {
		buffer.push_back(pos[i * 3 + 0]);
		buffer.push_back(pos[i * 3 + 1]);
		buffer.push_back(pos[i * 3 + 2]);

		if (flags & VAO_NORMALS) {
			buffer.push_back(normal[0]);
			buffer.push_back(normal[1]);
			buffer.push_back(normal[2]);
		}

		if (flags & VAO_UVS) {
			buffer.push_back(pos[i * 3 + 0]);
			buffer.push_back(pos[i * 3 + 2]);
		}
	}

	indicies.push_back(i++);
	indicies.push_back(i++);
	indicies.push_back(i++);
}

void VertexArrayObject::addTriangleUVs(std::array<GLfloat, 15> data) {

	int i = (int)buffer.size() / stride;

	std::vector<GLfloat> normal;
	if (flags & VAO_NORMALS) normal = calculateNormal(std::vector<GLfloat>({ data[0], data[1], data[2], data[5], data[6], data[7], data[10], data[11], data[12] }));

	for (int i = 0; i < 3; i++) {
		buffer.push_back(data[i * 5 + 0]);
		buffer.push_back(data[i * 5 + 1]);
		buffer.push_back(data[i * 5 + 2]);

		if (flags & VAO_NORMALS) {
			buffer.push_back(normal[0]);
			buffer.push_back(normal[1]);
			buffer.push_back(normal[2]);
		}

		if (flags & VAO_UVS) {
			buffer.push_back(data[i * 5 + 3]);
			buffer.push_back(data[i * 5 + 4]);
		}
	}

	indicies.push_back(i++);
	indicies.push_back(i++);
	indicies.push_back(i++);
}

void VertexArrayObject::addTriangleUVs(std::array<std::array<GLfloat, 5>, 3> data) {

	int i = (int)buffer.size() / stride;

	std::vector<GLfloat> normal;
	if (flags & VAO_NORMALS) normal = calculateNormal(std::vector<GLfloat>({ data[0][0], data[0][1], data[0][2], data[1][0], data[1][1], data[1][2], data[2][0], data[2][1], data[2][2] }));

	for (int i = 0; i < 3; i++) {
		buffer.push_back(data[i][0]);
		buffer.push_back(data[i][1]);
		buffer.push_back(data[i][2]);

		if (flags & VAO_NORMALS) {
			buffer.push_back(normal[0]);
			buffer.push_back(normal[1]);
			buffer.push_back(normal[2]);
		}

		if (flags & VAO_UVS) {
			buffer.push_back(data[i][3]);
			buffer.push_back(data[i][4]);
		}
	}

	indicies.push_back(i++);
	indicies.push_back(i++);
	indicies.push_back(i++);
}

void VertexArrayObject::addTriangleWithNormals(std::array<std::array<GLfloat, 6>, 3> data) {

	int i = (int)buffer.size() / stride;

	for (int i = 0; i < 3; i++) {
		buffer.push_back(data[i][0]);
		buffer.push_back(data[i][1]);
		buffer.push_back(data[i][2]);

		if (flags & VAO_NORMALS) {
			buffer.push_back(data[i][3]);
			buffer.push_back(data[i][4]);
			buffer.push_back(data[i][5]);
		}

		if (flags & VAO_UVS) {
			buffer.push_back(data[i][0]);
			buffer.push_back(data[i][2]);
		}
	}

	indicies.push_back(i++);
	indicies.push_back(i++);
	indicies.push_back(i++);
}

void VertexArrayObject::addTriangleWithNormals(std::array<GLfloat, 6 * 3> data) {

	int i = (int)buffer.size() / stride;

	for (int i = 0; i < 3; i++) {
		buffer.push_back(data[i * 6 + 0]);
		buffer.push_back(data[i * 6 + 1]);
		buffer.push_back(data[i * 6 + 2]);

		if (flags & VAO_NORMALS) {
			buffer.push_back(data[i * 6 + 3]);
			buffer.push_back(data[i * 6 + 4]);
			buffer.push_back(data[i * 6 + 5]);
		}

		if (flags & VAO_UVS) {
			buffer.push_back(data[i * 6 + 0]);
			buffer.push_back(data[i * 6 + 2]);
		}
	}

	indicies.push_back(i++);
	indicies.push_back(i++);
	indicies.push_back(i++);
}

void VertexArrayObject::addTriangelUVsNormals(std::array<GLfloat, 3 * (3 + 3 + 2)> data) {

	int i = (int)buffer.size() / stride;

	std::vector<GLfloat> normal;

	for (int i = 0; i < 3; i++) {
		buffer.push_back(data[i * 8 + 0]);
		buffer.push_back(data[i * 8 + 1]);
		buffer.push_back(data[i * 8 + 2]);

		if (flags & VAO_NORMALS) {
			buffer.push_back(data[i * 8 + 3]);
			buffer.push_back(data[i * 8 + 4]);
			buffer.push_back(data[i * 8 + 5]);
		}

		if (flags & VAO_UVS) {
			buffer.push_back(data[i * 8 + 6]);
			buffer.push_back(data[i * 8 + 7]);
		}
	}

	indicies.push_back(i++);
	indicies.push_back(i++);
	indicies.push_back(i++);
}

void VertexArrayObject::addQuad(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, GLfloat x3, GLfloat y3, GLfloat z3, GLfloat x4, GLfloat y4, GLfloat z4) {
	
	int i = (int)buffer.size() / stride;

	std::vector<GLfloat> normal;
	if (flags & VAO_NORMALS) normal = calculateNormal({ x1, y1, z1, x2, y2, z2, x3, y3, z3 });

	buffer.push_back(x1);
	buffer.push_back(y1);
	buffer.push_back(z1);

	if (flags & VAO_NORMALS) {
		buffer.push_back(normal[0]);
		buffer.push_back(normal[1]);
		buffer.push_back(normal[2]);
	}

	if (flags & VAO_UVS) {
		buffer.push_back(x1);
		buffer.push_back(z1);
	}

	buffer.push_back(x2);
	buffer.push_back(y2);
	buffer.push_back(z2);

	if (flags & VAO_NORMALS) {
		buffer.push_back(normal[0]);
		buffer.push_back(normal[1]);
		buffer.push_back(normal[2]);
	}

	if (flags & VAO_UVS) {
		buffer.push_back(x2);
		buffer.push_back(z2);
	}

	buffer.push_back(x3);
	buffer.push_back(y3);
	buffer.push_back(z3);

	if (flags & VAO_NORMALS) {
		buffer.push_back(normal[0]);
		buffer.push_back(normal[1]);
		buffer.push_back(normal[2]);
	}

	if (flags & VAO_UVS) {
		buffer.push_back(x3);
		buffer.push_back(z3);
	}

	buffer.push_back(x4);
	buffer.push_back(y4);
	buffer.push_back(z4);

	if (flags & VAO_NORMALS) {
		buffer.push_back(normal[0]);
		buffer.push_back(normal[1]);
		buffer.push_back(normal[2]);
	}

	if (flags & VAO_UVS) {
		buffer.push_back(x4);
		buffer.push_back(z4);
	}

	indicies.push_back(i + 0);
	indicies.push_back(i + 1);
	indicies.push_back(i + 3);

	indicies.push_back(i + 1);
	indicies.push_back(i + 2);
	indicies.push_back(i + 3);
}

void VertexArrayObject::addQuad(std::array<GLfloat, 12> pos) {
	
	int i = (int)buffer.size() / stride;

	std::vector<GLfloat> normal;
	if (flags & VAO_NORMALS) normal = calculateNormal(std::vector<GLfloat>(pos.begin(), pos.end()));

	for (int i = 0; i < 4; i++) {
		buffer.push_back(pos[i * 3 + 0]);
		buffer.push_back(pos[i * 3 + 1]);
		buffer.push_back(pos[i * 3 + 2]);

		if (flags & VAO_NORMALS) {
			buffer.push_back(normal[0]);
			buffer.push_back(normal[1]);
			buffer.push_back(normal[2]);
		}

		if (flags & VAO_UVS) {
			buffer.push_back(pos[i * 3 + 0]);
			buffer.push_back(pos[i * 3 + 2]);
		}
	}

	indicies.push_back(i + 0);
	indicies.push_back(i + 1);
	indicies.push_back(i + 3);

	indicies.push_back(i + 1);
	indicies.push_back(i + 2);
	indicies.push_back(i + 3);
}

void VertexArrayObject::addQuadWithNormals(std::array<GLfloat, 24> data) {

	int i = (int)buffer.size() / stride;

	for (int i = 0; i < 4; i++) {
		buffer.push_back(data[i * 6 + 0]);
		buffer.push_back(data[i * 6 + 1]);
		buffer.push_back(data[i * 6 + 2]);

		if (flags & VAO_NORMALS) {
			buffer.push_back(data[i * 6 + 3]);
			buffer.push_back(data[i * 6 + 4]);
			buffer.push_back(data[i * 6 + 5]);
		}

		if (flags & VAO_UVS) {
			buffer.push_back(data[i * 6 + 0]);
			buffer.push_back(data[i * 6 + 2]);
		}
	}

	indicies.push_back(i + 0);
	indicies.push_back(i + 1);
	indicies.push_back(i + 3);

	indicies.push_back(i + 1);
	indicies.push_back(i + 2);
	indicies.push_back(i + 3);
}

void VertexArrayObject::addQuadWithUVs(std::array<GLfloat, 20> data) {

	int i = (int)buffer.size() / stride;

	std::vector<GLfloat> normal;
	if (flags & VAO_NORMALS) normal = calculateNormal(std::vector<GLfloat>({ data[0], data[1], data[2], data[5], data[6], data[7], data[15], data[16], data[17] }));

	for (int i = 0; i < 4; i++) {
		buffer.push_back(data[i * 5 + 0]);
		buffer.push_back(data[i * 5 + 1]);
		buffer.push_back(data[i * 5 + 2]);

		if (flags & VAO_NORMALS) {
			buffer.push_back(normal[0]);
			buffer.push_back(normal[1]);
			buffer.push_back(normal[2]);
		}

		if (flags & VAO_UVS) {
			buffer.push_back(data[i * 5 + 3]);
			buffer.push_back(data[i * 5 + 4]);
		}
	}

	indicies.push_back(i + 0);
	indicies.push_back(i + 1);
	indicies.push_back(i + 3);

	indicies.push_back(i + 1);
	indicies.push_back(i + 2);
	indicies.push_back(i + 3);
}

void VertexArrayObject::addQuadUVsNormals(std::array<GLfloat, 4 * (3 + 3 + 2)> data) {

	int i = (int)buffer.size() / stride;

	std::vector<GLfloat> normal;

	for (int i = 0; i < 4; i++) {
		buffer.push_back(data[i * 8 + 0]);
		buffer.push_back(data[i * 8 + 1]);
		buffer.push_back(data[i * 8 + 2]);

		if (flags & VAO_NORMALS) {
			buffer.push_back(data[i * 8 + 3]);
			buffer.push_back(data[i * 8 + 4]);
			buffer.push_back(data[i * 8 + 5]);
		}

		if (flags & VAO_UVS) {
			buffer.push_back(data[i * 8 + 6]);
			buffer.push_back(data[i * 8 + 7]);
		}
	}

	indicies.push_back(i + 0);
	indicies.push_back(i + 1);
	indicies.push_back(i + 3);

	indicies.push_back(i + 1);
	indicies.push_back(i + 2);
	indicies.push_back(i + 3);
}

void VertexArrayObject::addCuboid(std::array<GLfloat, 24> pos) {
	
	int i = (int)buffer.size() / 6;

	for (int i = 0; i < 24; i++) {
		buffer.push_back(pos[i]);
		if (i % 3 == 2) {
			buffer.push_back(pos[i-2]);
			buffer.push_back(pos[i-1]);
			buffer.push_back(pos[i]);
		}
	}

	indicies.push_back(i + 0);
	indicies.push_back(i + 1);
	indicies.push_back(i + 3);

	indicies.push_back(i + 1);
	indicies.push_back(i + 2);
	indicies.push_back(i + 3);


	indicies.push_back(i + 7);
	indicies.push_back(i + 6);
	indicies.push_back(i + 4);

	indicies.push_back(i + 6);
	indicies.push_back(i + 5);
	indicies.push_back(i + 4);


	indicies.push_back(i + 4);
	indicies.push_back(i + 5);
	indicies.push_back(i + 0);

	indicies.push_back(i + 5);
	indicies.push_back(i + 1);
	indicies.push_back(i + 0);


	indicies.push_back(i + 5);
	indicies.push_back(i + 6);
	indicies.push_back(i + 1);

	indicies.push_back(i + 6);
	indicies.push_back(i + 2);
	indicies.push_back(i + 1);


	indicies.push_back(i + 6);
	indicies.push_back(i + 7);
	indicies.push_back(i + 2);

	indicies.push_back(i + 7);
	indicies.push_back(i + 3);
	indicies.push_back(i + 2);


	indicies.push_back(i + 7);
	indicies.push_back(i + 4);
	indicies.push_back(i + 3);

	indicies.push_back(i + 4);
	indicies.push_back(i + 0);
	indicies.push_back(i + 3);
}

void VertexArrayObject::addPolygon(std::vector<std::array<GLfloat, 3>> points) {
	if (points.size() < 3) return;
	if (points.size() == 3) {
		addTriangle(points[0][0], points[0][1], points[0][2], points[1][0], points[1][1], points[1][2], points[2][0], points[2][1], points[2][2]);
		return;
	}
	Vec3f dir;
	for (int i = 0; i < points.size(); i++) {
		Vec3f a = points[i];
		Vec3f b = points[(i + 1) % points.size()];
		Vec3f c = points[(i + 2) % points.size()];
		Vec3f d = ((b - a) ^ (c - b)) / ((c - b)() * (b - a)());
		float ang = asin(d());
		ang = ((b - a) * (c - b)) >= 0.0f ? ang : ang < 0.0f ? -(float)M_PI - ang : (float)M_PI - ang;
		dir += d / d() * ang;
	}
	for (int i = 0; i < points.size(); i++) {
		Vec3f a = points[i];
		Vec3f b = points[(i + 1) % points.size()];
		Vec3f c = points[(i + 2) % points.size()];
		if (((b - a) ^ (c - b)) * dir > 0) {
			for (int j = i + 3; j < points.size() + i; j++) {
				size_t k = j >= points.size() ? j - points.size() : j;
				Vec3f d = points[k];
				float e = ((b - a) ^ (d - a)) * dir;
				float f = ((c - b) ^ (d - b)) * dir;
				float g = ((a - c) ^ (d - c)) * dir;
				if (e > 0 && f > 0 && g > 0) goto Label;
			}
			addTriangle(a[0], a[1], a[2], b[0], b[1], b[2], c[0], c[1], c[2]);
			points.erase(points.begin() + (i + 1) % points.size());
			addPolygon(points);
			return;
		}
		Label:
		continue;
	}
}

struct Comparator {
	const std::vector<std::vector<std::array<float, 3>>> &contours;
	constexpr bool operator() (const std::pair<size_t, size_t> &left, const std::pair<size_t, size_t> &right) {
		return contours[left.first][left.second][1] > contours[right.first][right.second][1];
	}
};

void VertexArrayObject::addComplexPolygon(const std::vector<std::vector<std::array<float, 3>>> &contours) {

	std::set<std::pair<size_t, size_t>, Comparator> localTops(Comparator{ contours });

	for (size_t contourI = 0; contourI < contours.size(); contourI++) {
		const std::vector<std::array<float, 3>> &contour = contours[contourI];
		bool lastUp = contour[contour.size() - 1][1] < contour[0][1];
		for (size_t point = 0; point < contour.size(); point++) {
			size_t nextPoint = point == contour.size() - 1 ? 0 : point + 1;
			bool nowDown = contour[point][1] > contour[nextPoint][1];
			bool top = lastUp && nowDown;
			if (top) {
				localTops.emplace(contourI, point);
			}
		}
	}

	//std::vector<size_t> contourTops;

	//size_t topContour = 0;

	//for (size_t contour = 0; contour < contours.size(); contour++) {
	//	size_t maxIndex = 0;
	//	for (size_t i = 0; i < contours[contour].size(); i++) {
	//		if (contours[contour][i][1] > contours[contour][maxIndex][1]) {
	//			maxIndex = i;
	//		}
	//	}
	//	contourTops.push_back(maxIndex);
	//	if (contours[contour][contourTops[contour]][1] > contours[topContour][contourTops[topContour]][1]) {
	//		topContour = contour;
	//	}
	//}

	//std::vector<std::pair<size_t, size_t>> monotonePolygon;

	//for (size_t contour = 0; contour < contours.size(); contour++) {
	//}
}

void VertexArrayObject::addPolygonWithUVs(std::vector<std::array<GLfloat, 5>> points) {
	if (points.size() < 3) return;
	if (points.size() == 3) {
		addTriangleUVs({ points[0], points[1], points[2] });
		return;
	}
	Vec3f dir;
	for (int i = 0; i < points.size(); i++) {
		Vec3f a({ points[i][0], points[i][1], points[i][2] });
		Vec3f b({ points[(i + 1) % points.size()][0], points[(i + 1) % points.size()][1], points[(i + 1) % points.size()][2] });
		Vec3f c({ points[(i + 2) % points.size()][0], points[(i + 2) % points.size()][1], points[(i + 2) % points.size()][2] });
		Vec3f d = ((b - a) ^ (c - b)) / ((c - b)() * (b - a)());
		if (d(2) != 0) {
			float ang = asin(d());
			ang = ((b - a) * (c - b)) >= 0.0f ? ang : ang <.0f ? -(float)M_PI - ang : (float)M_PI - ang;
			dir += d / d() * ang;
		}
	}
	for (int i = 0; i < points.size(); i++) {
		Vec3f a({ points[i][0], points[i][1], points[i][2] });
		Vec3f b({ points[(i + 1) % points.size()][0], points[(i + 1) % points.size()][1], points[(i + 1) % points.size()][2] });
		Vec3f c({ points[(i + 2) % points.size()][0], points[(i + 2) % points.size()][1], points[(i + 2) % points.size()][2] });
		if (((b - a) ^ (c - b)) * dir > 0) {
			for (int j = i + 3; j < points.size() + i; j++) {
				size_t k = j >= points.size() ? j - points.size() : j;
				Vec3f d({ points[k][0], points[k][1], points[k][2] });
				float e = ((b - a) ^ (d - a)) * dir;
				float f = ((c - b) ^ (d - b)) * dir;
				float g = ((a - c) ^ (d - c)) * dir;
				if (e > 0 && f > 0 && g > 0) goto Label;
			}
			addTriangleUVs({ points[i], points[(i + 1) % points.size()], points[(i + 2) % points.size()] });
			points.erase(points.begin() + (i + 1) % points.size());
			addPolygonWithUVs(points);
			return;
		}
	Label:
		continue;
	}
}

void VertexArrayObject::addPolygonWithNormals(std::vector<std::array<float, 6>> points) {
	if (points.size() < 3) return;
	if (points.size() == 3) {
		addTriangle(points[0][0], points[0][1], points[0][2], points[1][0], points[1][1], points[1][2], points[2][0], points[2][1], points[2][2]);
		return;
	}
	Vec3f dir;
	for (int i = 0; i < points.size(); i++) {
		Vec3f a({ points[i][0], points[i][1], points[i][2] });
		Vec3f b({ points[(i + 1) % points.size()][0], points[(i + 1) % points.size()][1], points[(i + 1) % points.size()][2] });
		Vec3f c({ points[(i + 2) % points.size()][0], points[(i + 2) % points.size()][1], points[(i + 2) % points.size()][2] });
		Vec3f d = ((b - a) ^ (c - b)) / ((c - b)() * (b - a)());
		if (d(2) != 0) {
			float ang = asin(d());
			ang = ((b - a) * (c - b)) >= 0.0f ? ang : ang < 0.0f ? -(float)M_PI - ang : (float)M_PI - ang;
			dir += d / d() * ang;
		}
	}
	for (int i = 0; i < points.size(); i++) {
		Vec3f a({ points[i][0], points[i][1], points[i][2] });
		Vec3f b({ points[(i + 1) % points.size()][0], points[(i + 1) % points.size()][1], points[(i + 1) % points.size()][2] });
		Vec3f c({ points[(i + 2) % points.size()][0], points[(i + 2) % points.size()][1], points[(i + 2) % points.size()][2] });
		if (((b - a) ^ (c - b)) * dir > 0) {
			for (int j = i + 3; j < points.size() + i; j++) {
				size_t k = j >= points.size() ? j - points.size() : j;
				Vec3f d({ points[k][0], points[k][1], points[k][2] });
				float e = ((b - a) ^ (d - a)) * dir;
				float f = ((c - b) ^ (d - b)) * dir;
				float g = ((a - c) ^ (d - c)) * dir;
				if (e > 0 && f > 0 && g > 0) goto Label;
			}
			addTriangleWithNormals({ points[i], points[(i + 1) % points.size()], points[(i + 2) % points.size()] });
			points.erase(points.begin() + (i + 1) % points.size());
			addPolygonWithNormals(points);
			return;
		}
	Label:
		continue;
	}
}

void VertexArrayObject::addHeightMesh(float x, float z, float width, float height, std::vector<std::vector<GLfloat>> heightMap) {

	int index = (int)buffer.size() / stride;

	for (int j = 1; j < heightMap[0].size() - 1; j++) {
		for (int i = 1; i < heightMap.size() - 1; i++) {
			buffer.push_back((float)(i - 1) / ((float)heightMap.size() - 3.0f) * width + x);
			buffer.push_back(heightMap[i][j]);
			buffer.push_back((float)(j - 1) / ((float)heightMap[0].size() - 3.0f) * height + z);

			if (flags & VAO_NORMALS) {
				float x0 = (float)(i - 1) / ((float)heightMap.size() - 3.0f) * width;
				float z0 = (float)(j - 1) / ((float)heightMap[0].size() - 3.0f) * height;
				float x1 = (float)(i + 1) / ((float)heightMap.size() - 3.0f) * width;
				float z1 = (float)(j + 1) / ((float)heightMap[0].size() - 3.0f) * height;
				float y10 = heightMap[i + 1][j];
				float y_0 = heightMap[i - 1][j];
				float y01 = heightMap[i][j + 1];
				float y0_ = heightMap[i][j - 1];
				std::array<GLfloat, 3> normal = normalize(cross({ x1 - x0, 0.0f, y10 - y_0 }, { 0.0f, z1 - z0, y01 - y0_ }));
				buffer.push_back(normal[0]);
				buffer.push_back(normal[1]);
				buffer.push_back(normal[2]);
			}

			if (flags & VAO_UVS) {
				buffer.push_back((float)(i - 1) / ((float)heightMap.size() - 3.0f) * width + x);
				buffer.push_back((float)(j - 1) / ((float)heightMap[0].size() - 3.0f) * height + z);
			}
		}
	}

	for (int y = 0; y < heightMap[0].size() - 3; y++) {
		for (int x = 0; x < heightMap.size() - 3; x++) {
			indicies.push_back(index + y * ((GLuint)heightMap.size() - 2) + x);
			indicies.push_back(index + y * ((GLuint)heightMap.size() - 2) + x + 1);
			indicies.push_back(index + (y + 1) * ((GLuint)heightMap.size() - 2) + x + 1);

			indicies.push_back(index + (y + 1) * ((GLuint)heightMap.size() - 2) + x + 1);
			indicies.push_back(index + (y + 1) * ((GLuint)heightMap.size() - 2) + x);
			indicies.push_back(index + y * ((GLuint)heightMap.size() - 2) + x);
		}
	}

}

int xyzToIndex(int x, int y, int z, int sizeX, int sizeY) {
	return (x + y * sizeX + z * sizeX * sizeY);
}

int rgbToInt(GLubyte r, GLubyte g, GLubyte b) {
	return r | (g << 8) | (b << 16);
}

void VertexArrayObject::bindBuffers() {

	//glActiveTexture(GL_TEXTURE0);

	//glBindTexture(GL_TEXTURE_3D, textureID);

	//const int sizeX = 4, sizeY = 32, sizeZ = 4;

	//GLuint textureData[sizeX * sizeY * sizeZ];

	////FourierNoiseGenerator red(10, 3, 1), green(10, 3, 2), blue(10, 3, 3);

	//PerlinNoise noise({ sizeX, sizeY, sizeZ }, SDL_GetTicks(), -1, 0.75f);

	//for (int i = 0; i < sizeX; i++) {
	//	for (int j = 0; j < sizeY; j++) {
	//		for (int k = 0; k < sizeZ; k++) {
	//			//textureData[xyzToIndex(i, j, k, sizeX, sizeY)] = rgbToInt((GLubyte)(255.0 * (sin((double)i / sizeX*3.14))), (GLubyte)(255.0 * (sin((double)j / sizeY*3.14))), (GLubyte)(255.0 * (sin((double)k / sizeZ*3.14))));
	//			//textureData[xyzToIndex(i, j, k, sizeX, sizeY)] = rgbToInt(255 * red.get({ (float)i / sizeX, (float)j / sizeX, (float)k / sizeX }), 255 * green.get({ (float)i / sizeX, (float)j / sizeX, (float)k / sizeX }), 255 * blue.get({ (float)i / sizeX, (float)j / sizeX, (float)k / sizeX }));
	//			float f = noise.get({ (double)i, (double)j, (double)k });
	//			//std::cout << f << std::endl;
	//			//textureData[xyzToIndex(i, j, k, sizeX, sizeY)] = rgbToInt(255 * (0.5 + f * 0.5 * 0.7), 255 * (0.25 + 0.75 * f * 0.7), 255 * f * 0.7);
	//			textureData[xyzToIndex(i, j, k, sizeX, sizeY)] = rgbToInt(j > 20 ? 255 * (0.5 + 0.5 * f) : 0, j >= 12 ? 255 * (0.5 + 0.5 * f) : 0, j < 12 || j > 20 ? 255 * (0.5 + 0.5 * f) : 0);
	//		}
	//	}
	//}

	//glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, sizeX, sizeY, sizeZ, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);

	//glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);

	if (!gen) {
		glGenVertexArrays(1, &ID);
		glGenBuffers(1, &bufferID);
		glGenBuffers(1, &elementID);
		glGenTextures(1, &textureID);
		gen = true;

		glBindVertexArray(ID);

		glBindBuffer(GL_ARRAY_BUFFER, bufferID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * buffer.size(), buffer.data(), GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indicies.size(), indicies.data(), GL_DYNAMIC_DRAW);

		int offset = 0;

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * stride, nullptr);
		glEnableVertexAttribArray(0);
		offset += 3;
		if (flags & VAO_NORMALS) {
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * stride, (void*)(sizeof(GL_FLOAT) * offset));
			glEnableVertexAttribArray(1);
			offset += 3;
		}
		if (flags & VAO_UVS) {
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * stride, (void*)(sizeof(GL_FLOAT) * offset));
			glEnableVertexAttribArray(2);
			offset += 2;
		}
	}
	else {
		glBindVertexArray(ID);

		glBindBuffer(GL_ARRAY_BUFFER, bufferID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * buffer.size(), buffer.data(), GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indicies.size(), indicies.data(), GL_DYNAMIC_DRAW);

		glBindVertexArray(0);
	}

	//glBindVertexArray(0);
}

void VertexArrayObject::copy(VertexArrayObject & other) {
	this->buffer = other.buffer;
	this->indicies = other.indicies;
	this->bindBuffers();
}

void VertexArrayObject::render() {
	glBindVertexArray(ID);
	glDrawElements(GL_TRIANGLES, (GLsizei) indicies.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

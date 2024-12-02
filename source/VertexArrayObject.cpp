#include "VertexArrayObject.h"

#include <SDL.h>

#include "FontRenderer.h"

namespace PiE {

	const VertexAttributeType POSITION_F(Usage::POSITION, VEC3);
	const VertexAttributeType NORMAL_F(Usage::NORMAL, VEC3);
	const VertexAttributeType COLOR3_F(Usage::COLOR, VEC3);
	const VertexAttributeType UV2_F(Usage::UV, VEC2);

	const VertexAttributeType POSITION_D(Usage::POSITION, VEC3D);
	const VertexAttributeType NORMAL_D(Usage::NORMAL, VEC3D);
	const VertexAttributeType COLOR3_D(Usage::COLOR, VEC3D);
	const VertexAttributeType UV2_D(Usage::UV, VEC2D);

	const VertexFormat VertexArrayObject::default_format{ POSITION_F };
	const VertexFormat Font::defaultFormat{ POSITION_F, UV2_F };

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

	VertexArrayObject::VertexArrayObject(VertexFormat format) : format(format) {}

	std::unordered_map<GLenum, std::vector<char>(*)(const char*, const char*, const char*)> calcNormal {
		{GL_FLOAT, [](const char* d1, const char* d2, const char* d3) {
			const GLfloat * v1 = (const GLfloat*)d1;
			const GLfloat * v2 = (const GLfloat*)d2;
			const GLfloat * v3 = (const GLfloat*)d3;

			GLfloat v4[3];
			GLfloat v5[3];
			GLfloat n[3];

			v4[0] = v1[0] - v2[0];
			v4[1] = v1[1] - v2[1];
			v4[2] = v1[2] - v2[2];

			v5[0] = v3[0] - v2[0];
			v5[1] = v3[1] - v2[1];
			v5[2] = v3[2] - v2[2];

			n[0] = v5[1] * v4[2] - v5[2] * v4[1];
			n[1] = v5[2] * v4[0] - v5[0] * v4[2];
			n[2] = v5[0] * v4[1] - v5[1] * v4[0];

			return std::vector<char>((char*)n, (char*)n + sizeof(n));
		}},
		{GL_DOUBLE, [](const char* d1, const char* d2, const char* d3) {
			const GLdouble * v1 = (const GLdouble*)d1;
			const GLdouble * v2 = (const GLdouble*)d2;
			const GLdouble * v3 = (const GLdouble*)d3;

			GLdouble v4[3];
			GLdouble v5[3];
			GLdouble n[3];

			v4[0] = v1[0] - v2[0];
			v4[1] = v1[1] - v2[1];
			v4[2] = v1[2] - v2[2];

			v5[0] = v3[0] - v2[0];
			v5[1] = v3[1] - v2[1];
			v5[2] = v3[2] - v2[2];

			n[0] = v5[1] * v4[2] - v5[2] * v4[1];
			n[1] = v5[2] * v4[0] - v5[0] * v4[2];
			n[2] = v5[0] * v4[1] - v5[1] * v4[0];

			return std::vector<char>((char*)n, (char*)n + sizeof(n));
		}}
	};

	std::unordered_map<GLenum, std::vector<char>(*)(const char*)> calcUV {
		{GL_FLOAT, [](const char* d1) {
			const GLfloat * v1 = (const GLfloat*)d1;

			GLfloat uv[2];

			uv[0] = v1[0];
			uv[1] = v1[2];

			return std::vector<char>((char*)uv, (char*)uv + sizeof(uv));
		}},
		{GL_DOUBLE, [](const char* d1) {
			const GLdouble * v1 = (const GLdouble*)d1;

			GLdouble uv[2];

			uv[0] = v1[0];
			uv[1] = v1[2];

			return std::vector<char>((char*)uv, (char*)uv + sizeof(uv));
		}}
	};

	std::unordered_map<GLenum, std::vector<char>(*)(std::vector<std::vector<char>>)> avgNormals {
		{GL_FLOAT, [](std::vector<std::vector<char>> d) {
			GLfloat avg[3]{ 0, 0, 0 };

			for (const std::vector<char> & dn : d) {
				const GLfloat * v = (const GLfloat*)dn.data();
				avg[0] += v[0];
				avg[1] += v[1];
				avg[2] += v[2];
			}

			GLfloat div = 1.0f / (avg[0] * avg[0] + avg[1] * avg[1] + avg[2] * avg[2]);

			avg[0] *= div;
			avg[1] *= div;
			avg[2] *= div;

			return std::vector<char>((char*)avg, (char*)avg + sizeof(avg));
		}},
		{GL_DOUBLE, [](std::vector<std::vector<char>> d) {
			GLdouble avg[3]{ 0, 0, 0 };

			for (const std::vector<char> & dn : d) {
				const GLdouble * v = (const GLdouble*)dn.data();
				avg[0] += v[0];
				avg[1] += v[1];
				avg[2] += v[2];
			}

			GLdouble div = 1.0f / (avg[0] * avg[0] + avg[1] * avg[1] + avg[2] * avg[2]);

			avg[0] *= div;
			avg[1] *= div;
			avg[2] *= div;

			return std::vector<char>((char*)avg, (char*)avg + sizeof(avg));
		}}
	};

	template<typename T>
	std::vector<char> getPolygonDirFunc(std::vector<std::vector<char>> d) {
		Vec<T, 3> dir{ { 0, 0, 0 } };
		for (int i = 0; i < d.size(); i++) {
			T * v1 = (T*)d[i].data();
			T * v2 = (T*)d[(i + 1) % d.size()].data();
			T * v3 = (T*)d[(i + 2) % d.size()].data();
			Vec<T, 3> a({ v1[0], v1[1], v1[2] });
			Vec<T, 3> b({ v2[0], v2[1], v2[2] });
			Vec<T, 3> c({ v3[0], v3[1], v3[2] });
			Vec<T, 3> d = ((b - a) ^ (c - b)) / ((c - b)() * (b - a)());
			if (d(2) != 0) {
				T ang = asin(d());
				ang = ((b - a) * (c - b)) >= 0.0f ? ang : ang < .0f ? -(T)M_PI - ang : (T)M_PI - ang;
				dir += d / d() * ang;
			}
		}
		char * dout = (char*)((std::array<T, 3Ui64>)dir).data();
		return std::vector<char>{ dout, dout + sizeof(T) * 3 };
	}

	std::unordered_map<GLenum, std::vector<char>(*)(std::vector<std::vector<char>>)> getPolygonDir {
		{GL_FLOAT, getPolygonDirFunc<GLfloat>},
		{GL_DOUBLE, getPolygonDirFunc<GLdouble>}
	};

	template<typename T>
	std::vector<GLuint> getPolygonIndicesFunc(std::vector<std::vector<char>> d, std::vector<char> dird) {
		std::vector<GLuint> indices;
		T * dirv = (T*)dird.data();
		Vec<T, 3> dir{ {dirv[0], dirv[1], dirv[2]} };
	Start:
		if (d.size() < 3) goto End;
		for (int i = 0; i < d.size(); i++) {
			T * v1 = (T*)d[i].data();
			T * v2 = (T*)d[(i + 1) % d.size()].data();
			T * v3 = (T*)d[(i + 2) % d.size()].data();
			Vec<T, 3> a({ v1[0], v1[1], v1[2] });
			Vec<T, 3> b({ v2[0], v2[1], v2[2] });
			Vec<T, 3> c({ v3[0], v3[1], v3[2] });
			if (((b - a) ^ (c - b)) * dir > 0) {
				for (int j = i + 3; j < d.size() + i; j++) {
					size_t k = j >= d.size() ? j - d.size() : j;
					T * v4 = (T*)d[k].data();
					Vec<T, 3> d({ v4[0], v4[1], v4[2] });
					T e = ((b - a) ^ (d - a)) * dir;
					T f = ((c - b) ^ (d - b)) * dir;
					T g = ((a - c) ^ (d - c)) * dir;
					if (e > 0 && f > 0 && g > 0) goto Continue;
				}
				indices.push_back(i);
				indices.push_back((i + 1) % d.size());
				indices.push_back((i + 2) % d.size());
				d.erase(d.begin() + (i + 1) % d.size());
				goto Start;
			}
		Continue:
			continue;
		}
	End:
		return indices;
	}

	std::unordered_map<GLenum, std::vector<GLuint>(*)(std::vector<std::vector<char>>, std::vector<char>)> getPolygonIndices {
		{GL_FLOAT, getPolygonIndicesFunc<GLfloat>},
		{GL_DOUBLE, getPolygonIndicesFunc<GLdouble>}
	};

	std::vector<char> getNormal(Vertex v1, Vertex v2, Vertex v3) {
		size_t p = v1.format.position_index, n = v1.format.normal_index;
		if (p >= 0 && n >= 0 && container_of(v1.format.types[p].type) == container_of(v1.format.types[n].type) && count_of(v1.format.types[p].type) == 3 && count_of(v1.format.types[p].type) == count_of(v1.format.types[n].type)) {
			size_t offset = v1.format.offsets[v1.format.position_index];
			return calcNormal[container_of(v1.format.types[v1.format.position_index].type)](v1.data + offset, v2.data + offset, v3.data + offset);
		}
		return std::vector<char>();
	}

	std::vector<char> getUV(Vertex v1) {
		size_t p = v1.format.position_index, uv = v1.format.uv_index;
		if (p >= 0 && uv >= 0 && container_of(v1.format.types[p].type) == container_of(v1.format.types[uv].type) && count_of(v1.format.types[p].type) == 3 && count_of(v1.format.types[uv].type) == 2) {
			size_t offset = v1.format.offsets[v1.format.position_index];
			return calcUV[container_of(v1.format.types[v1.format.position_index].type)](v1.data + offset);
		}
		return std::vector<char>();
	}

	#include "Lazy.h"

	void insertVertex(const Vertex & v, PiE::Lazy<std::vector<char>> & norm, VertexArrayObject & VAO) {
		for (size_t i = 0; i < VAO.format.types.size(); i++) {
			if (v.exists[i]) {
				VAO.buffer.insert(VAO.buffer.end(), v.data + VAO.format.offsets[i], v.data + VAO.format.offsets[i] + VAO.format.types[i].size);
			}
			else {
				std::vector<char> data;

				switch (VAO.format.types[i].usage) {
				case Usage::NORMAL:
					data = norm;
				case Usage::UV:
					data = getUV(v);
				}

				if (data.empty()) {
					VAO.buffer.insert(VAO.buffer.end(), VAO.format.types[i].default_data, VAO.format.types[i].default_data + VAO.format.types[i].size);
				}
				else {
					VAO.buffer.insert(VAO.buffer.end(), data.begin(), data.end());
				}
			}
		}
	}

	void VertexArrayObject::addTriangle(Vertex v1, Vertex v2, Vertex v3) {
		if (v1.format == v2.format && v2.format == v3.format && v3.format == format) {

			size_t index = buffer.size() / format.stride;

			PiE::Lazy<std::vector<char>> norm{ [&v1, &v2, &v3]() { return getNormal(v1, v2, v3); } };

			insertVertex(v1, norm, *this);
			insertVertex(v2, norm, *this);
			insertVertex(v3, norm, *this);

			indicies.push_back((GLuint)index++);
			indicies.push_back((GLuint)index++);
			indicies.push_back((GLuint)index++);

		}
		else {
			puts("Missmatched Vertex Formats");
		}
	}

	//void VertexArrayObject::addTriangle(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, GLfloat x3, GLfloat y3, GLfloat z3) {
	//	
	//	size_t i = buffer.size() / format.stride;
	//
	//	bool normals = flags & VAO_NORMALS;
	//	bool uvs = flags & VAO_UVS;
	//	std::vector<GLfloat> normal;
	//
	//	if (normals) normal = calculateNormal({x1, y1, z1, x2, y2, z2, x3, y3, z3});
	//
	//	buffer.push_back(x1);
	//	buffer.push_back(y1);
	//	buffer.push_back(z1);
	//
	//	if (normals) {
	//		buffer.push_back(normal[0]);
	//		buffer.push_back(normal[1]);
	//		buffer.push_back(normal[2]);
	//	}
	//
	//	if (uvs) {
	//		buffer.push_back(x1);
	//		buffer.push_back(z1);
	//	}
	//
	//	buffer.push_back(x2);
	//	buffer.push_back(y2);
	//	buffer.push_back(z2);
	//
	//	if (normals) {
	//		buffer.push_back(normal[0]);
	//		buffer.push_back(normal[1]);
	//		buffer.push_back(normal[2]);
	//	}
	//
	//	if (uvs) {
	//		buffer.push_back(x2);
	//		buffer.push_back(z2);
	//	}
	//
	//	buffer.push_back(x3);
	//	buffer.push_back(y3);
	//	buffer.push_back(z3);
	//
	//	if (normals) {
	//		buffer.push_back(normal[0]);
	//		buffer.push_back(normal[1]);
	//		buffer.push_back(normal[2]);
	//	}
	//
	//	if (uvs) {
	//		buffer.push_back(x3);
	//		buffer.push_back(z3);
	//	}
	//
	//	indicies.push_back(i++);
	//	indicies.push_back(i++);
	//	indicies.push_back(i++);
	//}

	//void VertexArrayObject::addTriangle(std::array<GLfloat, 9> pos) {
	//
	//	int i = (int)buffer.size() / stride;
	//
	//	std::vector<GLfloat> normal;
	//	if (flags & VAO_NORMALS) normal = calculateNormal(std::vector<GLfloat>(pos.begin(), pos.end()));
	//
	//	for (int i = 0; i < 3; i++) {
	//		buffer.push_back(pos[i * 3 + 0]);
	//		buffer.push_back(pos[i * 3 + 1]);
	//		buffer.push_back(pos[i * 3 + 2]);
	//
	//		if (flags & VAO_NORMALS) {
	//			buffer.push_back(normal[0]);
	//			buffer.push_back(normal[1]);
	//			buffer.push_back(normal[2]);
	//		}
	//
	//		if (flags & VAO_UVS) {
	//			buffer.push_back(pos[i * 3 + 0]);
	//			buffer.push_back(pos[i * 3 + 2]);
	//		}
	//	}
	//
	//	indicies.push_back(i++);
	//	indicies.push_back(i++);
	//	indicies.push_back(i++);
	//}

	//void VertexArrayObject::addTriangleUVs(std::array<GLfloat, 15> data) {
	//
	//	int i = (int)buffer.size() / stride;
	//
	//	std::vector<GLfloat> normal;
	//	if (flags & VAO_NORMALS) normal = calculateNormal(std::vector<GLfloat>({ data[0], data[1], data[2], data[5], data[6], data[7], data[10], data[11], data[12] }));
	//
	//	for (int i = 0; i < 3; i++) {
	//		buffer.push_back(data[i * 5 + 0]);
	//		buffer.push_back(data[i * 5 + 1]);
	//		buffer.push_back(data[i * 5 + 2]);
	//
	//		if (flags & VAO_NORMALS) {
	//			buffer.push_back(normal[0]);
	//			buffer.push_back(normal[1]);
	//			buffer.push_back(normal[2]);
	//		}
	//
	//		if (flags & VAO_UVS) {
	//			buffer.push_back(data[i * 5 + 3]);
	//			buffer.push_back(data[i * 5 + 4]);
	//		}
	//	}
	//
	//	indicies.push_back(i++);
	//	indicies.push_back(i++);
	//	indicies.push_back(i++);
	//}

	//void VertexArrayObject::addTriangleUVs(std::array<std::array<GLfloat, 5>, 3> data) {
	//
	//	int i = (int)buffer.size() / stride;
	//
	//	std::vector<GLfloat> normal;
	//	if (flags & VAO_NORMALS) normal = calculateNormal(std::vector<GLfloat>({ data[0][0], data[0][1], data[0][2], data[1][0], data[1][1], data[1][2], data[2][0], data[2][1], data[2][2] }));
	//
	//	for (int i = 0; i < 3; i++) {
	//		buffer.push_back(data[i][0]);
	//		buffer.push_back(data[i][1]);
	//		buffer.push_back(data[i][2]);
	//
	//		if (flags & VAO_NORMALS) {
	//			buffer.push_back(normal[0]);
	//			buffer.push_back(normal[1]);
	//			buffer.push_back(normal[2]);
	//		}
	//
	//		if (flags & VAO_UVS) {
	//			buffer.push_back(data[i][3]);
	//			buffer.push_back(data[i][4]);
	//		}
	//	}
	//
	//	indicies.push_back(i++);
	//	indicies.push_back(i++);
	//	indicies.push_back(i++);
	//}

	//void VertexArrayObject::addTriangleWithNormals(std::array<std::array<GLfloat, 6>, 3> data) {
	//
	//	int i = (int)buffer.size() / stride;
	//
	//	for (int i = 0; i < 3; i++) {
	//		buffer.push_back(data[i][0]);
	//		buffer.push_back(data[i][1]);
	//		buffer.push_back(data[i][2]);
	//
	//		if (flags & VAO_NORMALS) {
	//			buffer.push_back(data[i][3]);
	//			buffer.push_back(data[i][4]);
	//			buffer.push_back(data[i][5]);
	//		}
	//
	//		if (flags & VAO_UVS) {
	//			buffer.push_back(data[i][0]);
	//			buffer.push_back(data[i][2]);
	//		}
	//	}
	//
	//	indicies.push_back(i++);
	//	indicies.push_back(i++);
	//	indicies.push_back(i++);
	//}

	//void VertexArrayObject::addTriangleWithNormals(std::array<GLfloat, 6 * 3> data) {
	//
	//	int i = (int)buffer.size() / stride;
	//
	//	for (int i = 0; i < 3; i++) {
	//		buffer.push_back(data[i * 6 + 0]);
	//		buffer.push_back(data[i * 6 + 1]);
	//		buffer.push_back(data[i * 6 + 2]);
	//
	//		if (flags & VAO_NORMALS) {
	//			buffer.push_back(data[i * 6 + 3]);
	//			buffer.push_back(data[i * 6 + 4]);
	//			buffer.push_back(data[i * 6 + 5]);
	//		}
	//
	//		if (flags & VAO_UVS) {
	//			buffer.push_back(data[i * 6 + 0]);
	//			buffer.push_back(data[i * 6 + 2]);
	//		}
	//	}
	//
	//	indicies.push_back(i++);
	//	indicies.push_back(i++);
	//	indicies.push_back(i++);
	//}

	//void VertexArrayObject::addTriangelUVsNormals(std::array<GLfloat, 3 * (3 + 3 + 2)> data) {
	//
	//	int i = (int)buffer.size() / stride;
	//
	//	std::vector<GLfloat> normal;
	//
	//	for (int i = 0; i < 3; i++) {
	//		buffer.push_back(data[i * 8 + 0]);
	//		buffer.push_back(data[i * 8 + 1]);
	//		buffer.push_back(data[i * 8 + 2]);
	//
	//		if (flags & VAO_NORMALS) {
	//			buffer.push_back(data[i * 8 + 3]);
	//			buffer.push_back(data[i * 8 + 4]);
	//			buffer.push_back(data[i * 8 + 5]);
	//		}
	//
	//		if (flags & VAO_UVS) {
	//			buffer.push_back(data[i * 8 + 6]);
	//			buffer.push_back(data[i * 8 + 7]);
	//		}
	//	}
	//
	//	indicies.push_back(i++);
	//	indicies.push_back(i++);
	//	indicies.push_back(i++);
	//}

	void VertexArrayObject::addQuad(Vertex v1, Vertex v2, Vertex v3, Vertex v4) {
		if (v1.format == v2.format && v2.format == v3.format && v3.format == format && v4.format == format) {

			size_t index = buffer.size() / format.stride;

			PiE::Lazy<std::vector<char>> norm1{ [&v1, &v2, &v4]() { return getNormal(v1, v2, v4); } };
			PiE::Lazy<std::vector<char>> norm2{ [&v2, &v3, &v4]() { return getNormal(v2, v3, v4); } };
			PiE::Lazy<std::vector<char>> norm3{ [&norm1, &norm2, this]() -> std::vector<char> { return avgNormals[container_of(format.types[format.normal_index].type)]({ norm1(), norm2() }); } };

			insertVertex(v1, norm1, *this);
			insertVertex(v2, norm3, *this);
			insertVertex(v3, norm3, *this);
			insertVertex(v4, norm2, *this);

			indicies.push_back((GLuint)index + 0);
			indicies.push_back((GLuint)index + 1);
			indicies.push_back((GLuint)index + 3);

			indicies.push_back((GLuint)index + 1);
			indicies.push_back((GLuint)index + 2);
			indicies.push_back((GLuint)index + 3);

		}
		else {
			puts("Missmatched Vertex Formats");
		}
	}

	//void VertexArrayObject::addQuad(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, GLfloat x3, GLfloat y3, GLfloat z3, GLfloat x4, GLfloat y4, GLfloat z4) {
	//	
	//	int i = (int)buffer.size() / stride;
	//
	//	std::vector<GLfloat> normal;
	//	if (flags & VAO_NORMALS) normal = calculateNormal({ x1, y1, z1, x2, y2, z2, x3, y3, z3 });
	//
	//	buffer.push_back(x1);
	//	buffer.push_back(y1);
	//	buffer.push_back(z1);
	//
	//	if (flags & VAO_NORMALS) {
	//		buffer.push_back(normal[0]);
	//		buffer.push_back(normal[1]);
	//		buffer.push_back(normal[2]);
	//	}
	//
	//	if (flags & VAO_UVS) {
	//		buffer.push_back(x1);
	//		buffer.push_back(z1);
	//	}
	//
	//	buffer.push_back(x2);
	//	buffer.push_back(y2);
	//	buffer.push_back(z2);
	//
	//	if (flags & VAO_NORMALS) {
	//		buffer.push_back(normal[0]);
	//		buffer.push_back(normal[1]);
	//		buffer.push_back(normal[2]);
	//	}
	//
	//	if (flags & VAO_UVS) {
	//		buffer.push_back(x2);
	//		buffer.push_back(z2);
	//	}
	//
	//	buffer.push_back(x3);
	//	buffer.push_back(y3);
	//	buffer.push_back(z3);
	//
	//	if (flags & VAO_NORMALS) {
	//		buffer.push_back(normal[0]);
	//		buffer.push_back(normal[1]);
	//		buffer.push_back(normal[2]);
	//	}
	//
	//	if (flags & VAO_UVS) {
	//		buffer.push_back(x3);
	//		buffer.push_back(z3);
	//	}
	//
	//	buffer.push_back(x4);
	//	buffer.push_back(y4);
	//	buffer.push_back(z4);
	//
	//	if (flags & VAO_NORMALS) {
	//		buffer.push_back(normal[0]);
	//		buffer.push_back(normal[1]);
	//		buffer.push_back(normal[2]);
	//	}
	//
	//	if (flags & VAO_UVS) {
	//		buffer.push_back(x4);
	//		buffer.push_back(z4);
	//	}
	//
	//	indicies.push_back(i + 0);
	//	indicies.push_back(i + 1);
	//	indicies.push_back(i + 3);
	//
	//	indicies.push_back(i + 1);
	//	indicies.push_back(i + 2);
	//	indicies.push_back(i + 3);
	//}
	//
	//void VertexArrayObject::addQuad(std::array<GLfloat, 12> pos) {
	//	
	//	int i = (int)buffer.size() / stride;
	//
	//	std::vector<GLfloat> normal;
	//	if (flags & VAO_NORMALS) normal = calculateNormal(std::vector<GLfloat>(pos.begin(), pos.end()));
	//
	//	for (int i = 0; i < 4; i++) {
	//		buffer.push_back(pos[i * 3 + 0]);
	//		buffer.push_back(pos[i * 3 + 1]);
	//		buffer.push_back(pos[i * 3 + 2]);
	//
	//		if (flags & VAO_NORMALS) {
	//			buffer.push_back(normal[0]);
	//			buffer.push_back(normal[1]);
	//			buffer.push_back(normal[2]);
	//		}
	//
	//		if (flags & VAO_UVS) {
	//			buffer.push_back(pos[i * 3 + 0]);
	//			buffer.push_back(pos[i * 3 + 2]);
	//		}
	//	}
	//
	//	indicies.push_back(i + 0);
	//	indicies.push_back(i + 1);
	//	indicies.push_back(i + 3);
	//
	//	indicies.push_back(i + 1);
	//	indicies.push_back(i + 2);
	//	indicies.push_back(i + 3);
	//}
	//
	//void VertexArrayObject::addQuadWithNormals(std::array<GLfloat, 24> data) {
	//
	//	int i = (int)buffer.size() / stride;
	//
	//	for (int i = 0; i < 4; i++) {
	//		buffer.push_back(data[i * 6 + 0]);
	//		buffer.push_back(data[i * 6 + 1]);
	//		buffer.push_back(data[i * 6 + 2]);
	//
	//		if (flags & VAO_NORMALS) {
	//			buffer.push_back(data[i * 6 + 3]);
	//			buffer.push_back(data[i * 6 + 4]);
	//			buffer.push_back(data[i * 6 + 5]);
	//		}
	//
	//		if (flags & VAO_UVS) {
	//			buffer.push_back(data[i * 6 + 0]);
	//			buffer.push_back(data[i * 6 + 2]);
	//		}
	//	}
	//
	//	indicies.push_back(i + 0);
	//	indicies.push_back(i + 1);
	//	indicies.push_back(i + 3);
	//
	//	indicies.push_back(i + 1);
	//	indicies.push_back(i + 2);
	//	indicies.push_back(i + 3);
	//}
	//
	//void VertexArrayObject::addQuadWithUVs(std::array<GLfloat, 20> data) {
	//
	//	int i = (int)buffer.size() / stride;
	//
	//	std::vector<GLfloat> normal;
	//	if (flags & VAO_NORMALS) normal = calculateNormal(std::vector<GLfloat>({ data[0], data[1], data[2], data[5], data[6], data[7], data[15], data[16], data[17] }));
	//
	//	for (int i = 0; i < 4; i++) {
	//		buffer.push_back(data[i * 5 + 0]);
	//		buffer.push_back(data[i * 5 + 1]);
	//		buffer.push_back(data[i * 5 + 2]);
	//
	//		if (flags & VAO_NORMALS) {
	//			buffer.push_back(normal[0]);
	//			buffer.push_back(normal[1]);
	//			buffer.push_back(normal[2]);
	//		}
	//
	//		if (flags & VAO_UVS) {
	//			buffer.push_back(data[i * 5 + 3]);
	//			buffer.push_back(data[i * 5 + 4]);
	//		}
	//	}
	//
	//	indicies.push_back(i + 0);
	//	indicies.push_back(i + 1);
	//	indicies.push_back(i + 3);
	//
	//	indicies.push_back(i + 1);
	//	indicies.push_back(i + 2);
	//	indicies.push_back(i + 3);
	//}
	//
	//void VertexArrayObject::addQuadUVsNormals(std::array<GLfloat, 4 * (3 + 3 + 2)> data) {
	//
	//	int i = (int)buffer.size() / stride;
	//
	//	std::vector<GLfloat> normal;
	//
	//	for (int i = 0; i < 4; i++) {
	//		buffer.push_back(data[i * 8 + 0]);
	//		buffer.push_back(data[i * 8 + 1]);
	//		buffer.push_back(data[i * 8 + 2]);
	//
	//		if (flags & VAO_NORMALS) {
	//			buffer.push_back(data[i * 8 + 3]);
	//			buffer.push_back(data[i * 8 + 4]);
	//			buffer.push_back(data[i * 8 + 5]);
	//		}
	//
	//		if (flags & VAO_UVS) {
	//			buffer.push_back(data[i * 8 + 6]);
	//			buffer.push_back(data[i * 8 + 7]);
	//		}
	//	}
	//
	//	indicies.push_back(i + 0);
	//	indicies.push_back(i + 1);
	//	indicies.push_back(i + 3);
	//
	//	indicies.push_back(i + 1);
	//	indicies.push_back(i + 2);
	//	indicies.push_back(i + 3);
	//}

	void VertexArrayObject::addCuboid(Vertex v1, Vertex v2, Vertex v3, Vertex v4, Vertex v5, Vertex v6, Vertex v7, Vertex v8) {
		if (v1.format == v2.format && v2.format == v3.format && v3.format == format && v4.format == format) {

			size_t index = buffer.size() / format.stride;

			PiE::Lazy<std::vector<char>> norm1{ [&v4, &v3, &v1]() { return getNormal(v4, v3, v1); } }; // bottom
			PiE::Lazy<std::vector<char>> norm2{ [&v3, &v2, &v1]() { return getNormal(v3, v2, v1); } };
			PiE::Lazy<std::vector<char>> norm3{ [&v5, &v6, &v8]() { return getNormal(v5, v6, v8); } }; // top
			PiE::Lazy<std::vector<char>> norm4{ [&v6, &v7, &v8]() { return getNormal(v6, v7, v8); } };
			PiE::Lazy<std::vector<char>> norm5{ [&v1, &v2, &v5]() { return getNormal(v1, v2, v5); } }; // front
			PiE::Lazy<std::vector<char>> norm6{ [&v2, &v6, &v5]() { return getNormal(v2, v6, v5); } };
			PiE::Lazy<std::vector<char>> norm7{ [&v2, &v3, &v6]() { return getNormal(v2, v3, v6); } }; // right
			PiE::Lazy<std::vector<char>> norm8{ [&v3, &v7, &v6]() { return getNormal(v3, v7, v6); } };
			PiE::Lazy<std::vector<char>> norm9{ [&v3, &v4, &v7]() { return getNormal(v3, v4, v7); } }; // back
			PiE::Lazy<std::vector<char>> norm10{ [&v4, &v8, &v7]() { return getNormal(v4, v8, v7); } };
			PiE::Lazy<std::vector<char>> norm11{ [&v4, &v1, &v8]() { return getNormal(v4, v1, v8); } }; // left
			PiE::Lazy<std::vector<char>> norm12{ [&v1, &v5, &v8]() { return getNormal(v1, v5, v8); } };

			GLenum n_type = container_of(format.types[format.normal_index].type);
			PiE::Lazy<std::vector<char>> norm13{ [&norm11, &norm12, &norm5, &norm1, &norm2, n_type]() { return avgNormals[n_type]({ norm11(), norm12(), norm5(), norm1(), norm2() }); } };
			PiE::Lazy<std::vector<char>> norm14{ [&norm5, &norm6, &norm7, &norm2, n_type]() { return avgNormals[n_type]({ norm5(), norm6(), norm7(), norm2() }); } };
			PiE::Lazy<std::vector<char>> norm15{ [&norm7, &norm8, &norm9, &norm1, &norm2, n_type]() { return avgNormals[n_type]({ norm7(), norm8(), norm9(), norm1(), norm2() }); } };
			PiE::Lazy<std::vector<char>> norm16{ [&norm9, &norm10, &norm11, &norm1, n_type]() { return avgNormals[n_type]({ norm9(), norm10(), norm11(), norm1() }); } };
			PiE::Lazy<std::vector<char>> norm17{ [&norm12, &norm5, &norm6, &norm3, n_type]() { return avgNormals[n_type]({ norm12(), norm5(), norm6(), norm3 }); } };
			PiE::Lazy<std::vector<char>> norm18{ [&norm6, &norm7, &norm8, &norm3, &norm4, n_type]() { return avgNormals[n_type]({ norm6(), norm7(), norm8(), norm3(), norm4() }); } };
			PiE::Lazy<std::vector<char>> norm19{ [&norm8, &norm9, &norm10, &norm4, n_type]() { return avgNormals[n_type]({ norm8(), norm9(), norm10(), norm4() }); } };
			PiE::Lazy<std::vector<char>> norm20{ [&norm10, &norm11, &norm12, &norm3, &norm4, n_type]() { return avgNormals[n_type]({ norm10(), norm11(), norm12(), norm3(), norm4() }); } };

			insertVertex(v1, norm13, *this);
			insertVertex(v2, norm14, *this);
			insertVertex(v3, norm15, *this);
			insertVertex(v4, norm16, *this);
			insertVertex(v5, norm17, *this);
			insertVertex(v6, norm18, *this);
			insertVertex(v7, norm19, *this);
			insertVertex(v8, norm20, *this);

			indicies.push_back((GLuint)(index + 3)); // bottom
			indicies.push_back((GLuint)(index + 2));
			indicies.push_back((GLuint)(index + 0));

			indicies.push_back((GLuint)(index + 2));
			indicies.push_back((GLuint)(index + 1));
			indicies.push_back((GLuint)(index + 0));


			indicies.push_back((GLuint)(index + 4)); // top
			indicies.push_back((GLuint)(index + 5));
			indicies.push_back((GLuint)(index + 7));

			indicies.push_back((GLuint)(index + 5));
			indicies.push_back((GLuint)(index + 6));
			indicies.push_back((GLuint)(index + 7));


			indicies.push_back((GLuint)(index + 0)); // front
			indicies.push_back((GLuint)(index + 1));
			indicies.push_back((GLuint)(index + 4));

			indicies.push_back((GLuint)(index + 1));
			indicies.push_back((GLuint)(index + 5));
			indicies.push_back((GLuint)(index + 4));


			indicies.push_back((GLuint)(index + 1)); // right
			indicies.push_back((GLuint)(index + 2));
			indicies.push_back((GLuint)(index + 5));

			indicies.push_back((GLuint)(index + 2));
			indicies.push_back((GLuint)(index + 6));
			indicies.push_back((GLuint)(index + 5));


			indicies.push_back((GLuint)(index + 2)); // back
			indicies.push_back((GLuint)(index + 3));
			indicies.push_back((GLuint)(index + 6));

			indicies.push_back((GLuint)(index + 3));
			indicies.push_back((GLuint)(index + 7));
			indicies.push_back((GLuint)(index + 6));


			indicies.push_back((GLuint)(index + 3)); // left
			indicies.push_back((GLuint)(index + 0));
			indicies.push_back((GLuint)(index + 7));

			indicies.push_back((GLuint)(index + 0));
			indicies.push_back((GLuint)(index + 4));
			indicies.push_back((GLuint)(index + 7));

		}
		else {
			puts("Missmatched Vertex Formats");
		}
	}

	void VertexArrayObject::addCuboidQuads(Vertex v1, Vertex v2, Vertex v3, Vertex v4, Vertex v5, Vertex v6, Vertex v7, Vertex v8) {
		addQuad(v4, v3, v2, v1); // bottom
		addQuad(v5, v6, v7, v8); // top
		addQuad(v1, v2, v6, v5); // front
		addQuad(v2, v3, v7, v6); // right
		addQuad(v3, v4, v8, v7); // back
		addQuad(v4, v1, v5, v8); // left
	}

	//void VertexArrayObject::addCuboid(std::array<GLfloat, 24> pos) {
	//	
	//	int i = (int)buffer.size() / 6;
	//
	//	for (int i = 0; i < 24; i++) {
	//		buffer.push_back(pos[i]);
	//		if (i % 3 == 2) {
	//			buffer.push_back(pos[i-2]);
	//			buffer.push_back(pos[i-1]);
	//			buffer.push_back(pos[i]);
	//		}
	//	}
	//
	//	indicies.push_back(i + 0);
	//	indicies.push_back(i + 1);
	//	indicies.push_back(i + 3);
	//
	//	indicies.push_back(i + 1);
	//	indicies.push_back(i + 2);
	//	indicies.push_back(i + 3);
	//
	//
	//	indicies.push_back(i + 7);
	//	indicies.push_back(i + 6);
	//	indicies.push_back(i + 4);
	//
	//	indicies.push_back(i + 6);
	//	indicies.push_back(i + 5);
	//	indicies.push_back(i + 4);
	//
	//
	//	indicies.push_back(i + 4);
	//	indicies.push_back(i + 5);
	//	indicies.push_back(i + 0);
	//
	//	indicies.push_back(i + 5);
	//	indicies.push_back(i + 1);
	//	indicies.push_back(i + 0);
	//
	//
	//	indicies.push_back(i + 5);
	//	indicies.push_back(i + 6);
	//	indicies.push_back(i + 1);
	//
	//	indicies.push_back(i + 6);
	//	indicies.push_back(i + 2);
	//	indicies.push_back(i + 1);
	//
	//
	//	indicies.push_back(i + 6);
	//	indicies.push_back(i + 7);
	//	indicies.push_back(i + 2);
	//
	//	indicies.push_back(i + 7);
	//	indicies.push_back(i + 3);
	//	indicies.push_back(i + 2);
	//
	//
	//	indicies.push_back(i + 7);
	//	indicies.push_back(i + 4);
	//	indicies.push_back(i + 3);
	//
	//	indicies.push_back(i + 4);
	//	indicies.push_back(i + 0);
	//	indicies.push_back(i + 3);
	//}

	//void VertexArrayObject::addPolygon(std::vector<std::array<GLfloat, 3>> points) {
	//	if (points.size() < 3) return;
	//	if (points.size() == 3) {
	//		addTriangle(points[0][0], points[0][1], points[0][2], points[1][0], points[1][1], points[1][2], points[2][0], points[2][1], points[2][2]);
	//		return;
	//	}
	//	Vec3f dir;
	//	for (int i = 0; i < points.size(); i++) {
	//		Vec3f a = points[i];
	//		Vec3f b = points[(i + 1) % points.size()];
	//		Vec3f c = points[(i + 2) % points.size()];
	//		Vec3f d = ((b - a) ^ (c - b)) / ((c - b)() * (b - a)());
	//		float ang = asin(d());
	//		ang = ((b - a) * (c - b)) >= 0.0f ? ang : ang < 0.0f ? -(float)M_PI - ang : (float)M_PI - ang;
	//		dir += d / d() * ang;
	//	}
	//	for (int i = 0; i < points.size(); i++) {
	//		Vec3f a = points[i];
	//		Vec3f b = points[(i + 1) % points.size()];
	//		Vec3f c = points[(i + 2) % points.size()];
	//		if (((b - a) ^ (c - b)) * dir > 0) {
	//			for (int j = i + 3; j < points.size() + i; j++) {
	//				size_t k = j >= points.size() ? j - points.size() : j;
	//				Vec3f d = points[k];
	//				float e = ((b - a) ^ (d - a)) * dir;
	//				float f = ((c - b) ^ (d - b)) * dir;
	//				float g = ((a - c) ^ (d - c)) * dir;
	//				if (e > 0 && f > 0 && g > 0) goto Label;
	//			}
	//			addTriangle(a[0], a[1], a[2], b[0], b[1], b[2], c[0], c[1], c[2]);
	//			points.erase(points.begin() + (i + 1) % points.size());
	//			addPolygon(points);
	//			return;
	//		}
	//		Label:
	//		continue;
	//	}
	//}

	//struct Comparator {
	//	const std::vector<std::vector<std::array<float, 3>>> &contours;
	//	constexpr bool operator() (const std::pair<size_t, size_t> &left, const std::pair<size_t, size_t> &right) {
	//		return contours[left.first][left.second][1] > contours[right.first][right.second][1];
	//	}
	//};

	//void VertexArrayObject::addComplexPolygon(const std::vector<std::vector<std::array<float, 3>>> &contours) {
	//
	//	std::set<std::pair<size_t, size_t>, Comparator> localTops(Comparator{ contours });
	//
	//	for (size_t contourI = 0; contourI < contours.size(); contourI++) {
	//		const std::vector<std::array<float, 3>> &contour = contours[contourI];
	//		bool lastUp = contour[contour.size() - 1][1] < contour[0][1];
	//		for (size_t point = 0; point < contour.size(); point++) {
	//			size_t nextPoint = point == contour.size() - 1 ? 0 : point + 1;
	//			bool nowDown = contour[point][1] > contour[nextPoint][1];
	//			bool top = lastUp && nowDown;
	//			if (top) {
	//				localTops.emplace(contourI, point);
	//			}
	//		}
	//	}
	//
	//	//std::vector<size_t> contourTops;
	//
	//	//size_t topContour = 0;
	//
	//	//for (size_t contour = 0; contour < contours.size(); contour++) {
	//	//	size_t maxIndex = 0;
	//	//	for (size_t i = 0; i < contours[contour].size(); i++) {
	//	//		if (contours[contour][i][1] > contours[contour][maxIndex][1]) {
	//	//			maxIndex = i;
	//	//		}
	//	//	}
	//	//	contourTops.push_back(maxIndex);
	//	//	if (contours[contour][contourTops[contour]][1] > contours[topContour][contourTops[topContour]][1]) {
	//	//		topContour = contour;
	//	//	}
	//	//}
	//
	//	//std::vector<std::pair<size_t, size_t>> monotonePolygon;
	//
	//	//for (size_t contour = 0; contour < contours.size(); contour++) {
	//	//}
	//}

	void VertexArrayObject::addPolygon(std::vector<Vertex> vertices) {
		std::vector<std::vector<char>> positions;
		size_t p = format.position_index;
		if (container_size_of(format.types[p].type) != 3) {
			puts("Only Valid for 3D Objects");
			return;
		}
		for (const Vertex & vertex : vertices) {
			if (vertex.format != format) {
				puts("Missmatched Vertex Formats");
				return;
			}
			positions.push_back({ vertex.data + format.offsets[p], vertex.data + format.offsets[p] + format.types[p].size });
		}

		GLenum type = container_of(format.types[p].type);

		std::vector<char> dir = getPolygonDir[type](positions);

		std::vector<GLuint> indices = getPolygonIndices[type](positions, dir);

		addMesh(vertices, indices);
	}

	void VertexArrayObject::addMesh(const std::vector<Vertex> & vertices, const std::vector<GLuint> & indices) {
		std::vector<PiE::Lazy<std::vector<char>>> vertexNormals(vertices.size());
		std::unordered_map<size_t, std::vector<PiE::Lazy<std::vector<char>>*>> normals;
		std::vector<PiE::Lazy<std::vector<char>>> triangleNormals;

		triangleNormals.reserve(indices.size() / 3);

		for (int i = 0; i < indices.size(); i += 3) {
			PiE::Lazy<std::vector<char>> norm([&vertices, &indices, i]() {return getNormal(vertices[indices[i]], vertices[indices[i + 1]], vertices[indices[i + 2]]); });
			triangleNormals.push_back(norm);
			normals[indices[i + 0]].push_back(&triangleNormals.back());
			normals[indices[i + 1]].push_back(&triangleNormals.back());
			normals[indices[i + 2]].push_back(&triangleNormals.back());
		}

		size_t p = format.position_index;
		GLenum type = container_of(format.types[p].type);

		for (int i = 0; i < vertices.size(); i++) {
			PiE::Lazy<std::vector<char>> norm([type, i, &normals]() {
				std::vector<std::vector<char>> norms(normals[i].size());
				for (PiE::Lazy<std::vector<char>> * norm : normals[i]) {
					norms.push_back((*norm)());
				}
				return avgNormals[type](norms); 
			});
			vertexNormals.push_back(norm);
			insertVertex(vertices[i], vertexNormals.back(), *this);
		}

		size_t index_offset = indicies.size();

		for (const size_t & index : indices) {
			indicies.push_back(index + index_offset);
		}

	}

	//void VertexArrayObject::addPolygonWith_(std::vector<Vertex> vertices) {
	//	if (vertices.size() < 3) return;
	//	if (vertices.size() == 3) {
	//		addTriangle(vertices[0], vertices[1], vertices[2]);
	//		return;
	//	}
	//	Vec3f dir;
	//	for (int i = 0; i < vertices.size(); i++) {
	//		Vec3f a({ points[i][0], points[i][1], points[i][2] });
	//		Vec3f b({ points[(i + 1) % points.size()][0], points[(i + 1) % points.size()][1], points[(i + 1) % points.size()][2] });
	//		Vec3f c({ points[(i + 2) % points.size()][0], points[(i + 2) % points.size()][1], points[(i + 2) % points.size()][2] });
	//		Vec3f d = ((b - a) ^ (c - b)) / ((c - b)() * (b - a)());
	//		if (d(2) != 0) {
	//			float ang = asin(d());
	//			ang = ((b - a) * (c - b)) >= 0.0f ? ang : ang <.0f ? -(float)M_PI - ang : (float)M_PI - ang;
	//			dir += d / d() * ang;
	//		}
	//	}
	//	for (int i = 0; i < points.size(); i++) {
	//		Vec3f a({ points[i][0], points[i][1], points[i][2] });
	//		Vec3f b({ points[(i + 1) % points.size()][0], points[(i + 1) % points.size()][1], points[(i + 1) % points.size()][2] });
	//		Vec3f c({ points[(i + 2) % points.size()][0], points[(i + 2) % points.size()][1], points[(i + 2) % points.size()][2] });
	//		if (((b - a) ^ (c - b)) * dir > 0) {
	//			for (int j = i + 3; j < points.size() + i; j++) {
	//				size_t k = j >= points.size() ? j - points.size() : j;
	//				Vec3f d({ points[k][0], points[k][1], points[k][2] });
	//				float e = ((b - a) ^ (d - a)) * dir;
	//				float f = ((c - b) ^ (d - b)) * dir;
	//				float g = ((a - c) ^ (d - c)) * dir;
	//				if (e > 0 && f > 0 && g > 0) goto Label;
	//			}
	//			addTriangleUVs({ points[i], points[(i + 1) % points.size()], points[(i + 2) % points.size()] });
	//			points.erase(points.begin() + (i + 1) % points.size());
	//			addPolygonWithUVs(points);
	//			return;
	//		}
	//	Label:
	//		continue;
	//	}
	//}
	//
	//void VertexArrayObject::addPolygonWithNormals(std::vector<std::array<float, 6>> points) {
	//	if (points.size() < 3) return;
	//	if (points.size() == 3) {
	//		addTriangle(points[0][0], points[0][1], points[0][2], points[1][0], points[1][1], points[1][2], points[2][0], points[2][1], points[2][2]);
	//		return;
	//	}
	//	Vec3f dir;
	//	for (int i = 0; i < points.size(); i++) {
	//		Vec3f a({ points[i][0], points[i][1], points[i][2] });
	//		Vec3f b({ points[(i + 1) % points.size()][0], points[(i + 1) % points.size()][1], points[(i + 1) % points.size()][2] });
	//		Vec3f c({ points[(i + 2) % points.size()][0], points[(i + 2) % points.size()][1], points[(i + 2) % points.size()][2] });
	//		Vec3f d = ((b - a) ^ (c - b)) / ((c - b)() * (b - a)());
	//		if (d(2) != 0) {
	//			float ang = asin(d());
	//			ang = ((b - a) * (c - b)) >= 0.0f ? ang : ang < 0.0f ? -(float)M_PI - ang : (float)M_PI - ang;
	//			dir += d / d() * ang;
	//		}
	//	}
	//	for (int i = 0; i < points.size(); i++) {
	//		Vec3f a({ points[i][0], points[i][1], points[i][2] });
	//		Vec3f b({ points[(i + 1) % points.size()][0], points[(i + 1) % points.size()][1], points[(i + 1) % points.size()][2] });
	//		Vec3f c({ points[(i + 2) % points.size()][0], points[(i + 2) % points.size()][1], points[(i + 2) % points.size()][2] });
	//		if (((b - a) ^ (c - b)) * dir > 0) {
	//			for (int j = i + 3; j < points.size() + i; j++) {
	//				size_t k = j >= points.size() ? j - points.size() : j;
	//				Vec3f d({ points[k][0], points[k][1], points[k][2] });
	//				float e = ((b - a) ^ (d - a)) * dir;
	//				float f = ((c - b) ^ (d - b)) * dir;
	//				float g = ((a - c) ^ (d - c)) * dir;
	//				if (e > 0 && f > 0 && g > 0) goto Label;
	//			}
	//			addTriangleWithNormals({ points[i], points[(i + 1) % points.size()], points[(i + 2) % points.size()] });
	//			points.erase(points.begin() + (i + 1) % points.size());
	//			addPolygonWithNormals(points);
	//			return;
	//		}
	//	Label:
	//		continue;
	//	}
	//}

	//void VertexArrayObject::addHeightMesh(float x, float z, float width, float height, std::vector<std::vector<GLfloat>> heightMap) {
	//
	//	int index = (int)buffer.size() / stride;
	//
	//	for (int j = 1; j < heightMap[0].size() - 1; j++) {
	//		for (int i = 1; i < heightMap.size() - 1; i++) {
	//			float X = (float)(i - 1) / ((float)heightMap.size() - 3.0f) * width + x;
	//			float Y = (float)(j - 1) / ((float)heightMap[0].size() - 3.0f) * height + z;
	//			buffer.push_back(X);
	//			buffer.push_back(heightMap[i][j]);
	//			buffer.push_back(Y);
	//
	//			if (flags & VAO_NORMALS) {
	//				float x0 = (float)(i - 1) / ((float)heightMap.size() - 3.0f) * width;
	//				float z0 = (float)(j - 1) / ((float)heightMap[0].size() - 3.0f) * height;
	//				float x1 = (float)(i + 1) / ((float)heightMap.size() - 3.0f) * width;
	//				float z1 = (float)(j + 1) / ((float)heightMap[0].size() - 3.0f) * height;
	//				float y10 = heightMap[i + 1][j];
	//				float y_0 = heightMap[i - 1][j];
	//				float y01 = heightMap[i][j + 1];
	//				float y0_ = heightMap[i][j - 1];
	//				std::array<GLfloat, 3> normal = normalize(cross({ 0.0f, y01 - y0_, z1 - z0 }, { x1 - x0, y10 - y_0, 0.0f }));
	//				buffer.push_back(normal[0]);
	//				buffer.push_back(normal[1]);
	//				buffer.push_back(normal[2]);
	//			}
	//
	//			if (flags & VAO_UVS) {
	//				buffer.push_back((float)(i - 1) / ((float)heightMap.size() - 3.0f) * width + x);
	//				buffer.push_back((float)(j - 1) / ((float)heightMap[0].size() - 3.0f) * height + z);
	//			}
	//		}
	//	}
	//
	//	for (int y = 0; y < heightMap[0].size() - 3; y++) {
	//		for (int x = 0; x < heightMap.size() - 3; x++) {
	//			indicies.push_back(index + y * ((GLuint)heightMap.size() - 2) + x);
	//			indicies.push_back(index + y * ((GLuint)heightMap.size() - 2) + x + 1);
	//			indicies.push_back(index + (y + 1) * ((GLuint)heightMap.size() - 2) + x + 1);
	//
	//			indicies.push_back(index + (y + 1) * ((GLuint)heightMap.size() - 2) + x + 1);
	//			indicies.push_back(index + (y + 1) * ((GLuint)heightMap.size() - 2) + x);
	//			indicies.push_back(index + y * ((GLuint)heightMap.size() - 2) + x);
	//		}
	//	}
	//
	//}
	//
	//int xyzToIndex(int x, int y, int z, int sizeX, int sizeY) {
	//	return (x + y * sizeX + z * sizeX * sizeY);
	//}
	//
	//int rgbToInt(GLubyte r, GLubyte g, GLubyte b) {
	//	return r | (g << 8) | (b << 16);
	//}

	void VertexArrayObject::bindBuffers() {

		if (!gen) {
			glGenVertexArrays(1, &ID);
			glGenBuffers(1, &bufferID);
			glGenBuffers(1, &elementID);
			gen = true;

			glBindVertexArray(ID);

			glBindBuffer(GL_ARRAY_BUFFER, bufferID);
			glBufferData(GL_ARRAY_BUFFER, buffer.size(), buffer.data(), GL_DYNAMIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indicies.size(), indicies.data(), GL_DYNAMIC_DRAW);

			size_t offset = 0;

			for (size_t i = 0; i < format.types.size(); i++) {
				glVertexAttribPointer(i, format.types[i].count, container_of(format.types[i].type), GL_FALSE, format.stride, (void*) offset);
				glEnableVertexAttribArray(i);
				offset += format.types[i].size;
			}

			glBindVertexArray(0);
		}
		else {
			glBindVertexArray(ID);

			glBindBuffer(GL_ARRAY_BUFFER, bufferID);
			glBufferData(GL_ARRAY_BUFFER, buffer.size(), buffer.data(), GL_DYNAMIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indicies.size(), indicies.data(), GL_DYNAMIC_DRAW);

			glBindVertexArray(0);
		}
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

}
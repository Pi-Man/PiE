#include "Model4D.h"

namespace PiE {

	void Model4D::convertVAO(const VertexArrayObject & VAO) {
		size_t buffer_index = 0;
		while (buffer_index < VAO.buffer.size()) {
			Vertex v{ VAO.format, std::initializer_list<int>{} };
			v.format = VAO.format;
			memcpy(v.data, VAO.buffer.data() + buffer_index, VAO.format.stride);
			for (size_t i = 0; i < v.exists.size(); i++) v.exists[i] = true;
			points.push_back(v);
			buffer_index += VAO.format.stride;
		}
		for (const size_t & indice : VAO.indicies) {
			indices.push_back(indice);
		}
	}

	void Model4D::addTriangleIndices(int a, int b, int c) {
		indices.push_back(a);
		indices.push_back(b);
		indices.push_back(c);
	}

	void Model4D::addQuadIndices(int a, int b, int c, int d) {
		addTriangleIndices(a, b, d);
		addTriangleIndices(b, c, d);
	}

	void Model4D::addCuboidIndices(int a, int b, int c, int d, int e, int f, int g, int h) {
		addQuadIndices(a, b, c, d);
		addQuadIndices(h, g, f, e);
		addQuadIndices(e, f, b, a);
		addQuadIndices(f, g, c, b);
		addQuadIndices(g, h, d, c);
		addQuadIndices(h, e, a, d);
	}

	void Model4D::addHyperCuboidIndices(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m, int n, int o, int p) {
		addCuboidIndices(a, b, c, d, e, f, g, h);
		addCuboidIndices(i, j, k, l, m, n, o, p);
		addCuboidIndices(i, j, k, l, a, b, c, d);
		addCuboidIndices(e, f, g, h, m, n, o, p);
		addCuboidIndices(i, j, b, a, m, n, f, e);
		addCuboidIndices(b, j, k, c, f, n, o, g);
		addCuboidIndices(d, c, k, l, h, g, o, p);
		addCuboidIndices(i, a, d, l, m, e, h, p);
	}

	void Model4D::addTriangle(std::array<Vertex, 3> pointsIn) {
		int i = (int)points.size();

		points.push_back(pointsIn[0]);
		points.push_back(pointsIn[1]);
		points.push_back(pointsIn[2]);

		addTriangleIndices(
			i + 0,
			i + 1,
			i + 2
		);
	}

	void Model4D::addQuad(std::array<Vertex, 4> pointsIn) {
		int i = (int)points.size();

		points.push_back(pointsIn[0]);
		points.push_back(pointsIn[1]);
		points.push_back(pointsIn[2]);
		points.push_back(pointsIn[3]);

		addQuadIndices(
			i + 0,
			i + 1,
			i + 2,
			i + 3
		);
	}

	void Model4D::addCuboid(std::array<Vertex, 8> pos) {

		int i = (int)points.size();

		for (int i = 0; i < 8; i++) {
			points.push_back(pos[i]);
		}

		addCuboidIndices(
			i + 0,
			i + 1,
			i + 2,
			i + 3,
			i + 4,
			i + 5,
			i + 6,
			i + 7
		);
	}

	void Model4D::addHyperCuboid(std::array<Vertex, 16> pos) {

		int i = (int)points.size();

		for (int i = 0; i < 16; i++) {
			points.push_back(pos[i]);
		}

		addHyperCuboidIndices(
			i + 0,
			i + 1,
			i + 2,
			i + 3,
			i + 4,
			i + 5,
			i + 6,
			i + 7,
			i + 8,
			i + 9,
			i + 10,
			i + 11,
			i + 12,
			i + 13,
			i + 14,
			i + 15
		);
	}

	void Model4D::addCenteredHyperCuboid(double x, double y, double z, double w) {
		VertexFormat format{ POSITION_D };
		addHyperCuboid({ {
			{format, {x - 0.5, y - 0.5, z + 0.5, w - 0.5}},
			{format, {x + 0.5, y - 0.5, z + 0.5, w - 0.5}},
			{format, {x + 0.5, y + 0.5, z + 0.5, w - 0.5}},
			{format, {x - 0.5, y + 0.5, z + 0.5, w - 0.5}},
			{format, {x - 0.5, y - 0.5, z - 0.5, w - 0.5}},
			{format, {x + 0.5, y - 0.5, z - 0.5, w - 0.5}},
			{format, {x + 0.5, y + 0.5, z - 0.5, w - 0.5}},
			{format, {x - 0.5, y + 0.5, z - 0.5, w - 0.5}},
			{format, {x - 0.5, y - 0.5, z + 0.5, w + 0.5}},
			{format, {x + 0.5, y - 0.5, z + 0.5, w + 0.5}},
			{format, {x + 0.5, y + 0.5, z + 0.5, w + 0.5}},
			{format, {x - 0.5, y + 0.5, z + 0.5, w + 0.5}},
			{format, {x - 0.5, y - 0.5, z - 0.5, w + 0.5}},
			{format, {x + 0.5, y - 0.5, z - 0.5, w + 0.5}},
			{format, {x + 0.5, y + 0.5, z - 0.5, w + 0.5}},
			{format, {x - 0.5, y + 0.5, z - 0.5, w + 0.5}},
		} });
	}

}
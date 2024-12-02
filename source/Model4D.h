#ifndef _MODEL_4D_H
#define _MODEL_4D_H

#include <vector>

#include "Vec.h"
#include "Matrix.h"
#include "VertexArrayObject.h"

namespace PiE {

	struct Model4D {
		std::vector<Vertex> points;
		std::vector<size_t> indices;

		Matrix<double, 5> matrix = Matrix<double, 5>::Identity();

		void convertVAO(const VertexArrayObject & VAO);

		//  c
		// / \
		// a-b
		void addTriangleIndices(int a, int b, int c);

		// d-c
		// |\|
		// a-b
		void addQuadIndices(int a, int b, int c, int d);

		// d-c
		// | |\
		// a-b \
		//  \ h-g
		//   \|\|
		//    e-f
		void addCuboidIndices(int a, int b, int c, int d, int e, int f, int g, int h);

		//l  dc  k
		//i  ab\ j
		// p \hg  o
		// m  ef  n
		// inner cube, outer cube
		void addHyperCuboidIndices(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m, int n, int o, int p);

		void addTriangle(std::array<Vertex, 3> pointsIn);

		void addQuad(std::array<Vertex, 4> pointsIn);

		// 32
		// 01\
		// \76
		//  45
		void addCuboid(std::array<Vertex, 8> pos);

		//11 32  10
		//8  01\ 9
		// 15\76 14
		// 12 45 13
		void addHyperCuboid(std::array<Vertex, 16> pos);

		void addCenteredHyperCuboid(double x = 0, double y = 0, double z = 0, double w = 0);
	};

}

#endif
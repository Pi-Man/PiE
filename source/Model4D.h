#pragma once
#include <vector>
#include "Vec.h"
#include "Matrix.h"
struct Model4D
{
	std::vector<Vec4f> points;
	std::vector<size_t> indices;

	Matrix<float, 5> matrix = Matrix<float, 5>::Identity();

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

	void addTriangle(std::array<Vec4f, 3> pointsIn);

	void addQuad(std::array<Vec4f, 4> pointsIn);

	// 32
	// 01\
	// \76
	//  45
	void addCuboid(std::array<Vec4f, 8> pos);

	//11 32  10
	//8  01\ 9
	// 15\76 14
	// 12 45 13
	void addHyperCuboid(std::array<Vec4f, 16> pos);

	void addCenteredHyperCuboid(int x = 0, int y = 0, int z = 0, int w = 0);
};


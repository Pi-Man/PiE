
#include "obj.h"

inline std::array<double, 3> readVec3(const char *string) {
	char vertex_s[3][100];
	std::array<double, 3> vertex;
	int j = 0;
	int k = 0;
	for (int i = 0; i < 1000 && string[i] != 0; i++) {
		if ((string[i] >= '0' && string[i] <= '9') || string[i] == '.' || string[i] == '-') {
			vertex_s[j][k++] = string[i];
		}
		else if (string[i] == ' ' || string[i] == '\n') {
			vertex_s[j][k] = '\0';
			k = 0;
			if (vertex_s[j][0]) j++;
		}
	}
	for (int i = 0; i < 3; i++) {
		vertex[i] = atof(vertex_s[i]);
	}
	return vertex;
}

template<int N>
inline std::array<double, N> readVec(const char *string) {
	std::array<double, N> vertex;
	int j = 0;
	while (string[0]) {
		if (string[0] == ' ' && string[1] != ' ') {
			vertex[j] = atof(string);
			j++;
			if (j >= N) break;
		}
		string++;
	}
	return vertex;
}

inline std::vector<std::array<int, 3>> readFace(const char *string) {
	std::vector<std::array<char[100], 3>> indices_s;
	std::vector<std::array<int, 3>> indices;
	int j = 0;
	int k = 0;
	int l = 0;
	for (int i = 0; i < 1000 && string[i] != 0; i++) {
		if (indices_s.size() <= j) {
			indices_s.push_back(std::array<char[100], 3>());
		}
		if (string[i] >= '0' && string[i] <= '9') {
			indices_s[j][l][k++] = string[i];
		}
		else if (string[i] == '/') {
			indices_s[j][l][k] = '\0';
			k = 0;
			l++;
		}
		else if (string[i] == ' ' || string[i] == '\n') {
			indices_s[j][l][k] = '\0';
			k = 0;
			l = 0;
			if (indices_s[j][l][k]) j++;
		}
	}
	for (int i = 0; i < indices_s.size(); i++) {
		indices.push_back(std::array<int, 3>());
		for (int j = 0; j < 3; j++) {
			if (indices_s[i][j][0]) {
				indices[i][j] = atoi(indices_s[i][j]);
			}
			else {
				indices[i][j] = INT_MAX;
			}
		}
	}
	return indices;
}

inline std::vector<std::array<int, 3>> readFace2(const char *string) {
	std::vector<std::array<int, 3>> indices;
	indices.reserve(4);
	int j = -1;
	int l = 0;
	while (string[0]) {
		if (string[0] == '/') {
			if (string[1] != '/') {
				indices[j][l] = atof(string);
			}
			l++;
		}
		else if (string[0] == ' ' && string[1] != ' ') {
			j++;
			l = 0;
			indices.push_back({ INT_MAX, INT_MAX, INT_MAX });
			indices[j][l] = atof(string);
		}
		string++;
	}
	return indices;
}

inline void readFace2(const char *string, std::vector<std::array<int, 3>> &face) {
	face.clear();
	int j = -1;
	int l = 0;
	while (string[0]) {
		if (string[0] == '/') {
			l++;
			if (string[1] != '/') {
				face[j][l] = atof(string + 1);
			}
		}
		else if (string[0] == ' ' && string[1] != ' ') {
			j++;
			l = 0;
			face.push_back({ INT_MAX, INT_MAX, INT_MAX });
			face[j][l] = atof(string);
		}
		string++;
	}
}

template<int N>
inline std::array<GLfloat, N * 3> getVertexArray(const std::vector<std::array<double, 3>> &vertices, const std::vector<std::array<int, 3>> &indices) {
	std::array<GLfloat, N * 3> array;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < 3; j++) {
			array[i * 3 + j] = vertices[indices[i][0] - 1][j] * (j == 0 ? -1 : 1);
		}
	}
	return array;
}

inline std::vector<std::array<GLfloat, 3>> getVertexArray(int N, const std::vector<std::array<double, 3>> &vertices, const std::vector<std::array<int, 3>> &indices) {
	std::vector<std::array<GLfloat, 3>> vector;
	for (int i = 0; i < N; i++) {
		vector.push_back(std::array<GLfloat, 3>());
		for (int j = 0; j < 3; j++) {
			vector[i][j] = vertices[indices[i][0] - 1][j] * (j == 0 ? -1 : 1);
		}
	}
	return vector;
}

template<int N>
inline std::array<GLfloat, N * 3 * 2> getVertexNormalArray(const std::vector<std::array<double, 3>> &vertices, const std::vector<std::array<double, 3>> &normals, const std::vector<std::array<int, 3>> &indices) {
	std::array<GLfloat, N * 3 * 2> array;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < 3 * 2; j++) {
			if (j < 3) {
				array[i * 3 * 2 + j] = vertices[indices[i][0] - 1][j] * (j == 0 ? -1 : 1);
			}
			else {
				array[i * 3 * 2 + j] = normals[indices[i][2] - 1][j - 3] * (j == 3 ? -1 : 1);
			}
		}
	}
	return array;
}

inline std::vector<std::array<GLfloat, 3 * 2>> getVertexNormalArray(const int N, const std::vector<std::array<double, 3>> &vertices, const std::vector<std::array<double, 3>> &normals, const std::vector<std::array<int, 3>> &indices) {
	std::vector<std::array<GLfloat, 3 * 2>> vector;
	for (int i = 0; i < N; i++) {
		vector.push_back(std::array<GLfloat, 6>());
		for (int j = 0; j < 3 * 2; j++) {
			if (j < 3) {
				vector[i][j] = vertices[indices[i][0] - 1][j] * (j == 0 ? -1 : 1);
			}
			else {
				vector[i][j] = normals[indices[i][2] - 1][j - 3] * (j == 3 ? -1 : 1);
			}
		}
	}
	return vector;
}

template<int N>
inline std::array<GLfloat, N * (3 + 2)> getVertexUVsArray(const std::vector<std::array<double, 3>> &vertices, const std::vector<std::array<double, 3>> &uvs, const std::vector<std::array<int, 3>> &indices) {
	std::array<GLfloat, N * (3 + 2)> array;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < (3 + 2); j++) {
			if (j < 3) {
				array[i * (3 + 2) + j] = vertices[indices[i][0] - 1][j] * (j == 0 ? -1 : 1);
			}
			else {
				array[i * (3 + 2) + j] = uvs[indices[i][1] - 1][j - 3];
				if (j == 4) {
					array[i * (3 + 2) + j] = 1 - array[i * (3 + 2) + j];
				}
			}
		}
	}
	return array;
}

template<int N>
inline std::array<GLfloat, N * (3 + 3 + 2)> getVertexNormalsUVsArray(const std::vector<std::array<double, 3>> &vertices, const std::vector<std::array<double, 3>> normals, const std::vector<std::array<double, 3>> &uvs, const std::vector<std::array<int, 3>> &indices) {
	std::array<GLfloat, N * (3 + 3 + 2)> array;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < (3 + 3 + 2); j++) {
			if (j < 3) {
				array[i * (3 + 3 + 2) + j] = vertices[indices[i][0] - 1][j] * (j == 0 ? -1 : 1);
			}
			else if (j < 6) {
				array[i * (3 + 3 + 2) + j] = normals[indices[i][2] - 1][j - 3] * (j == 3 ? -1 : 1);
			}
			else {
				array[i * (3 + 3 + 2) + j] = uvs[indices[i][1] - 1][j - 6];
				if (j == 7) {
					array[i * (3 + 3 + 2) + j] = 1 - array[i * (3 + 3 + 2) + j];
				}
			}
		}
	}
	return array;
}

namespace OBJ {

	extern RenderContext blankMtl{};

	std::map<std::string, RenderContext> parseMtl(const char *fileName) {

		std::map<std::string, RenderContext> map = { {"none", blankMtl} };

		FILE *file;

		if (fopen_s(&file, fileName, "r")) {
			printf("Failed to open file: %s\n", fileName);
			return map;
		}

		std::string lastName;
		char _line[1001];
		char *line;
		Uint32 start = SDL_GetTicks();
		int i = 0;
		while (fgets(_line, 1001, file)) {
			line = _line;
			while (line[0] == ' ') {
				line++;
			}
			if (strncmp(line, "map_Kd", 5) == 0 && line[6] == ' ') {
				line += 7;
				while (line[0] == '-') {
					line++;
					if (strncmp(line, "blendu", 6)) {
						line += 7;
						while (line[0] != ' ') line++;
						line++;
					}
					else if (strncmp(line, "blendv", 6)) {
						line += 7;
						while (line[0] != ' ') line++;
						line++;
					}
					else if (strncmp(line, "cc", 2)) {
						line += 3;
						while (line[0] != ' ') line++;
						line++;
					}
					else if (strncmp(line, "clamp", 4)) {
						line += 5;
						while (line[0] != ' ') line++;
						line++;
					}
					else if (strncmp(line, "mm", 2)) {
						line += 3;
						while (line[0] != ' ') line++;
						line++;
						while (line[0] != ' ') line++;
						line++;
					}
					else if (line[0] == 'o') {
						line += 2;
						while (line[0] != ' ') line++;
						line++;
						while (line[0] != ' ') line++;
						line++;
						while (line[0] != ' ') line++;
						line++;
					}
					else if (line[0] == 's') {
						line += 2;
						while (line[0] != ' ') line++;
						line++;
						while (line[0] != ' ') line++;
						line++;
						while (line[0] != ' ') line++;
						line++;
					}
					else if (line[0] == 't') {
						line += 2;
						while (line[0] != ' ') line++;
						line++;
						while (line[0] != ' ') line++;
						line++;
						while (line[0] != ' ') line++;
						line++;
					}
					else if (strncmp(line, "texres", 6)) {
						line += 7;
						while (line[0] != ' ') line++;
						line++;
					}
					else {
						puts("Unknown Texture Argument!");
					}
				}
				int n = 0;
				while (line[n] != '\n') n++;
				line[n] = 0;
				map[lastName].texture = new GL_Texture2D(line);
				map[lastName].texture->generateMipMaps();
			}
			else if (strncmp(line, "newmtl ", 7) == 0) {
				line += 7;
				int n = 0;
				while (line[n] != '\n') n++;
				line[n] = 0;
				lastName = line;
				map.emplace(std::pair<std::string, RenderContext>({ lastName, RenderContext() }));
			}
		}

		fclose(file);
		return map;
	}

	std::vector<RenderObject> parseObj(const char *fileName, Uint32 flags) {

		std::map<std::string, RenderContext> matlib;

		std::vector<RenderObject> renderObjects = { RenderObject{VertexArrayObject(flags), Transform(), Transform(), Shader(), RenderContext()} };
		int objectID = 0;

		std::vector<std::array<double, 3>> vertices;
		std::vector<std::array<double, 3>> normals;
		std::vector<std::array<double, 3>> texcoords;
		std::vector<std::array<int, 3>> face;

		FILE *file;

		if (fopen_s(&file, fileName, "r")) {
			printf("Failed to open file: %s\n", fileName);
			return std::vector<RenderObject>();
		}

		char lastName[100];
		char _line[1001];
		char *line;
		Uint32 start = SDL_GetTicks();
		int i = 0;
		while (fgets(_line, 1001, file)) {
			line = _line;
			while (line[0] == ' ') {
				line++;
			}
			if (line[0] == 'v') {
				if (line[1] == ' ') {
					vertices.push_back(readVec<3>(line + 1));
				}
				else if (line[1] == 'n' && line[2] == ' ') {
					normals.push_back(readVec<3>(line + 2));
				}
				else if (line[1] == 't' && line[2] == ' ') {
					texcoords.push_back(readVec<3>(line + 2));
				}
			}
			else if (line[0] == 'o' && line[1] == ' ' && vertices.size() > 0) {
				objectID++;
				renderObjects.push_back(RenderObject{VertexArrayObject(flags), Transform(), Transform(), Shader(), RenderContext()});
				memcpy(lastName, line + 2, 100);
			}
			else if (strncmp(line, "mtllib", 6) == 0 && line[6] == ' ' && matlib.size() == 0) {
				line += 7;
				char fileName[100];
				int n = 0;
				while (line[n] != ' ' && line[n] != '\n') fileName[n] = line[n++];
				fileName[n] = 0;
				matlib = parseMtl(fileName);
			}
			else if (strncmp(line, "usemtl ", 7) == 0) {
				line += 7;
				int n = 0;
				while (line[n] != '\n') n++;
				line[n] = 0;
				auto it = matlib.find(line);
				renderObjects[objectID].renderContext = it == matlib.end() ? matlib["none"] : matlib[line];
			}
			else if (line[0] == 'f' && line[1] == ' ') {

				readFace2(line + 1, face);

				if (face.size() < 3) {
					puts("WARNING: face can not have fewer than 3 indices!");
				}
				else if (face.size() == 3) {
					if (face[0][1] != INT_MAX && face[0][2] == INT_MAX) {
						renderObjects[objectID].VAO.addTriangleUVs(getVertexUVsArray<3>(vertices, texcoords, face));
					}
					else if (face[0][1] == INT_MAX && face[0][2] != INT_MAX) {
						renderObjects[objectID].VAO.addTriangleWithNormals(getVertexNormalArray<3>(vertices, normals, face));
					}
					else if (face[0][1] != INT_MAX && face[0][2] != INT_MAX) {
						renderObjects[objectID].VAO.addTriangelUVsNormals(getVertexNormalsUVsArray<3>(vertices, normals, texcoords, face));
					}
					else {
						renderObjects[objectID].VAO.addTriangle(getVertexArray<3>(vertices, face));
					}
				}
				else if (face.size() == 4) {
					if (face[0][1] != INT_MAX && face[0][2] == INT_MAX) {
						renderObjects[objectID].VAO.addQuadWithUVs(getVertexUVsArray<4>(vertices, texcoords, face));
					}
					else if (face[0][1] == INT_MAX && face[0][2] != INT_MAX) {
						renderObjects[objectID].VAO.addQuadWithNormals(getVertexNormalArray<4>(vertices, normals, face));
					}
					else if (face[0][1] != INT_MAX && face[0][2] != INT_MAX) {
						renderObjects[objectID].VAO.addQuadUVsNormals(getVertexNormalsUVsArray<4>(vertices, normals, texcoords, face));
					}
					else {
						renderObjects[objectID].VAO.addQuad(getVertexArray<4>(vertices, face));
					}
				}
				else {
					if (face[0][1] != INT_MAX && face[0][2] == INT_MAX) {
						renderObjects[objectID].VAO.addPolygon(getVertexArray(face.size(), vertices, face));
					}
					else if (face[0][1] == INT_MAX && face[0][2] != INT_MAX) {
						renderObjects[objectID].VAO.addPolygonWithNormals(getVertexNormalArray(face.size(), vertices, normals, face));
					}
					else if (face[0][1] != INT_MAX && face[0][2] != INT_MAX) {
						renderObjects[objectID].VAO.addPolygonWithNormals(getVertexNormalArray(face.size(), vertices, normals, face));
					}
					else {
						renderObjects[objectID].VAO.addPolygon(getVertexArray(face.size(), vertices, face));
					}
				}
			}
			i++;
			if (SDL_GetTicks() > start + 1000) {
				printf("Parsed %6d lines\n", i);
				start += 1000;
			}
		}

		fclose(file);

		return renderObjects;
	}
}
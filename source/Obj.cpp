#include <cctype>

#include "obj.h"

namespace PiE {

	inline std::array<double, 3> readVec3(const char * string) {
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
	inline std::array<double, N> readVec(const char * string) {
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

	inline std::vector<std::array<int, 3>> readFace(const char * string) {
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

	inline std::vector<std::array<int, 3>> readFace2(const char * string) {
		std::vector<std::array<int, 3>> indices;
		indices.reserve(4);
		int j = -1;
		int l = 0;
		while (string[0]) {
			if (string[0] == '/') {
				if (string[1] != '/') {
					indices[j][l] = atoi(string);
				}
				l++;
			}
			else if (string[0] == ' ' && string[1] != ' ') {
				j++;
				l = 0;
				indices.push_back({ INT_MAX, INT_MAX, INT_MAX });
				indices[j][l] = atoi(string);
			}
			string++;
		}
		return indices;
	}

	inline void readFace2(const char * string, std::vector<std::array<size_t, 3>> & face) {
		face.clear();
		size_t j = -1;
		size_t l = 0;
		while (string[0]) {
			if (string[0] == '/') {
				l++;
				if (string[1] != '/') {
					face[j][l] = atoi(string + 1);
				}
			}
			else if (string[0] == ' ' && string[1] != ' ') {
				j++;
				l = 0;
				face.push_back({ INT_MAX, INT_MAX, INT_MAX });
				face[j][l] = atoi(string);
			}
			string++;
		}
	}

	template<size_t N>
	inline std::array<double, N * 3> getVertexArray(const std::vector<std::array<double, 3>> & vertices, const std::vector<std::array<size_t, 3>> & indices) {
		std::array<double, N * 3> array;
		for (size_t i = 0; i < N; i++) {
			for (size_t j = 0; j < 3; j++) {
				array[i * 3 + j] = (double)(vertices[indices[i][0] - 1][j] * (j == 0 ? -1 : 1));
			}
		}
		return array;
	}

	inline std::vector<std::array<double, 3>> getVertexArray(size_t N, const std::vector<std::array<double, 3>> & vertices, const std::vector<std::array<size_t, 3>> & indices) {
		std::vector<std::array<double, 3>> vector(N, std::array<double, 3>{});
		for (size_t i = 0; i < N; i++) {
			for (size_t j = 0; j < 3; j++) {
				vector[i][j] = (double)(vertices[indices[i][0] - 1][j] * (j == 0 ? -1 : 1));
			}
		}
		return vector;
	}

	template<size_t N>
	inline std::array<double, N * 3 * 2> getVertexNormalArray(const std::vector<std::array<double, 3>> & vertices, const std::vector<std::array<double, 3>> & normals, const std::vector<std::array<size_t, 3>> & indices) {
		std::array<double, N * 3 * 2> array;
		for (size_t i = 0; i < N; i++) {
			for (size_t j = 0; j < 3 * 2; j++) {
				if (j < 3) {
					array[i * 3 * 2 + j] = (double)(vertices[indices[i][0] - 1][j] * (j == 0 ? -1 : 1));
				}
				else {
					array[i * 3 * 2 + j] = (double)(normals[indices[i][2] - 1][j - 3] * (j == 3 ? -1 : 1));
				}
			}
		}
		return array;
	}

	inline std::vector<std::array<double, 3 * 2>> getVertexNormalArray(const size_t N, const std::vector<std::array<double, 3>> & vertices, const std::vector<std::array<double, 3>> & normals, const std::vector<std::array<size_t, 3>> & indices) {
		std::vector<std::array<double, 3 * 2>> vector(N, std::array<double, 3 * 2>{});
		for (size_t i = 0; i < N; i++) {
			for (size_t j = 0; j < 3 * 2; j++) {
				if (j < 3) {
					vector[i][j] = (double)(vertices[indices[i][0] - 1][j] * (j == 0 ? -1 : 1));
				}
				else {
					vector[i][j] = (double)(normals[indices[i][2] - 1][j - 3] * (j == 3 ? -1 : 1));
				}
			}
		}
		return vector;
	}

	template<size_t N>
	inline std::array<double, N * (3 + 2)> getVertexUVsArray(const std::vector<std::array<double, 3>> & vertices, const std::vector<std::array<double, 3>> & uvs, const std::vector<std::array<size_t, 3>> & indices) {
		std::array<double, N * (3 + 2)> array;
		for (size_t i = 0; i < N; i++) {
			for (size_t j = 0; j < (3 + 2); j++) {
				if (j < 3) {
					array[i * (3 + 2) + j] = (double)(vertices[indices[i][0] - 1][j] * (j == 0 ? -1 : 1));
				}
				else {
					array[i * (3 + 2) + j] = (double)(uvs[indices[i][1] - 1][j - 3]);
					if (j == 4) {
						array[i * (3 + 2) + j] = 1 - array[i * (3 + 2) + j];
					}
				}
			}
		}
		return array;
	}

	inline std::vector<std::array<double, 3 + 2>> getVertexUVsArray(const size_t N, const std::vector<std::array<double, 3>> & vertices, const std::vector<std::array<double, 3>> & uvs, const std::vector<std::array<size_t, 3>> & indices) {
		std::vector<std::array<double, 3 + 2>> vector(N, std::array<double, 3 + 2>{});
		for (size_t i = 0; i < N; i++) {
			for (size_t j = 0; j < (3 + 2); j++) {
				if (j < 3) {
					vector[i][j] = (double)(vertices[indices[i][0] - 1][j] * (j == 0 ? -1 : 1));
				}
				else {
					vector[i][j] = (double)(uvs[indices[i][1] - 1][j - 3]);
					if (j == 4) {
						vector[i][j] = 1 - vector[i][j];
					}
				}
			}
		}
		return vector;
	}

	template<size_t N>
	inline std::array<double, N * (3 + 3 + 2)> getVertexNormalsUVsArray(const std::vector<std::array<double, 3>> & vertices, const std::vector<std::array<double, 3>> normals, const std::vector<std::array<double, 3>> & uvs, const std::vector<std::array<size_t, 3>> & indices) {
		std::array<double, N * (3 + 3 + 2)> array;
		for (size_t i = 0; i < N; i++) {
			for (size_t j = 0; j < (3 + 3 + 2); j++) {
				if (j < 3) {
					array[i * (3 + 3 + 2) + j] = (double)(vertices[indices[i][0] - 1][j] * (j == 0 ? -1 : 1));
				}
				else if (j < 6) {
					array[i * (3 + 3 + 2) + j] = (double)(normals[indices[i][2] - 1][j - 3] * (j == 3 ? -1 : 1));
				}
				else {
					array[i * (3 + 3 + 2) + j] = (double)(uvs[indices[i][1] - 1][j - 6]);
					if (j == 7) {
						array[i * (3 + 3 + 2) + j] = 1 - array[i * (3 + 3 + 2) + j];
					}
				}
			}
		}
		return array;
	}

	inline std::vector<std::array<double, 3 + 3 + 2>> getVertexNormalsUVsArray(const size_t N, const std::vector<std::array<double, 3>> & vertices, const std::vector<std::array<double, 3>> normals, const std::vector<std::array<double, 3>> & uvs, const std::vector<std::array<size_t, 3>> & indices) {
		std::vector<std::array<double, 3 + 3 + 2>> vector(N, std::array<double, 3 + 3 + 2>{});
		for (size_t i = 0; i < N; i++) {
			for (size_t j = 0; j < (3 + 3 + 2); j++) {
				if (j < 3) {
					vector[i][j] = (double)(vertices[indices[i][0] - 1][j] * (j == 0 ? -1 : 1));
				}
				else if (j < 6) {
					vector[i][j] = (double)(normals[indices[i][2] - 1][j - 3] * (j == 3 ? -1 : 1));
				}
				else {
					vector[i][j] = (double)(uvs[indices[i][1] - 1][j - 6]);
					if (j == 7) {
						vector[i][j] = 1 - vector[i][j];
					}
				}
			}
		}
		return vector;
	}

	namespace OBJ {

		extern RenderContext blankMtl{};

		std::map<std::string, RenderContext> parseMtl(const char * fileName) {

			std::map<std::string, RenderContext> map = { {"none", blankMtl} };

			FILE * file;

			if (fopen_s(&file, fileName, "r")) {
				printf("Failed to open file: %s\n", fileName);
				return map;
			}

			std::string lastName;
			char _line[1001];
			char * line;
			Uint32 start = SDL_GetTicks();
			int i = 0;
			while (fgets(_line, 1001, file)) {
				line = _line;
				while (line[0] == ' ') {
					line++;
				}
				if (strncmp(line, "map_Kd", 6) == 0 && line[6] == ' ') {
					line += 7;
					bool
						blendu = true,
						blendv = true,
						colorCorrection = false,
						clamp = false
						;
					double
						base = 0,
						gain = 1,
						offsetU = 0,
						offsetV = 0,
						offsetW = 0,
						scaleU = 1,
						scaleV = 1,
						scaleW = 1,
						turbulenceU = 0,
						turbulenveV = 0,
						turbulenceW = 0
						;
					while (line[0] == '-') {
						line++;
						if (strncmp(line, "blendu", 6) == 0) {
							line += 7;
							if (strncmp(line, "on", 2) == 0) {
								line += 3;
							}
							else if (strncmp(line, "off", 3) == 0) {
								line += 4;
								blendu = false;
							}
						}
						else if (strncmp(line, "blendv", 6) == 0) {
							line += 7;
							if (strncmp(line, "on", 2) == 0) {
								line += 3;
							}
							else if (strncmp(line, "off", 3) == 0) {
								line += 4;
								blendv = false;
							}
						}
						else if (strncmp(line, "cc", 2) == 0) {
							line += 3;
							if (strncmp(line, "on", 2) == 0) {
								line += 3;
								colorCorrection = true;
							}
							else if (strncmp(line, "off", 3) == 0) {
								line += 4;
							}
						}
						else if (strncmp(line, "clamp", 4) == 0) {
							line += 5;
							if (strncmp(line, "on", 2) == 0) {
								line += 3;
								colorCorrection = true;
							}
							else if (strncmp(line, "off", 3) == 0) {
								line += 4;
							}
						}
						else if (strncmp(line, "mm", 2) == 0) {
							line += 2;
							const char * values = line;
							line++;
							while (line[0] != ' ') line++;
							line++;
							while (line[0] != ' ') line++;
							line++;
							std::array<double, 2> base_gain = readVec<2>(values);
							base = base_gain[0];
							gain = base_gain[1];
						}
						else if (line[0] == 'o') {
							line += 1;
							std::array<double, 2> offset = readVec<2>(line);
							offsetU = offset[0];
							offsetV = offset[1];
							if (std::isdigit(line[0])) {
								offsetW = readVec<1>(line)[0];
							}
						}
						else if (line[0] == 's') {
							line += 1;
							std::array<double, 2> scale = readVec<2>(line);
							scaleU = scale[0];
							scaleV = scale[1];
							if (std::isdigit(line[0])) {
								scaleW = readVec<1>(line)[0];
							}
						}
						else if (line[0] == 't') {
							line += 1;
							std::array<double, 2> turb = readVec<2>(line);
							turbulenceU = turb[0];
							turbulenveV = turb[1];
							if (std::isdigit(line[0])) {
								turbulenceW = readVec<1>(line)[0];
							}
						}
						else if (strncmp(line, "texres", 6) == 0) {
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
					map[lastName].texture = new GL_Texture2D(line, (blendu && blendv) ? GL_LINEAR : GL_NEAREST);
					if (base != 0 || gain != 1) {
						unsigned char * data = (unsigned char *)map[lastName].texture->data;
						for (int i = 0; i < map[lastName].texture->width * map[lastName].texture->height * 4; i++) {
							double value = data[i] * gain + base * 255;
							if (value > 255) value = 255;
							if (value < 0) value = 0;
							data[i] = (unsigned char)value;
						}
						map[lastName].texture->bindData();
					}
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

		std::vector<RenderObject> parseObj(const char * fileName, VertexFormat format) {

			std::map<std::string, RenderContext> matlib;

			std::vector<RenderObject> renderObjects = { RenderObject{VertexArrayObject(format), Transform(), Transform(), RenderContext()} };
			int objectID = 0;

			std::vector<std::array<double, 3>> vertices;
			std::vector<std::array<double, 3>> normals;
			std::vector<std::array<double, 3>> texcoords;
			std::vector<std::array<size_t, 3>> face;

			FILE * file;

			if (fopen_s(&file, fileName, "r")) {
				printf("Failed to open file: %s\n", fileName);
				return std::vector<RenderObject>();
			}

			char lastName[100];
			char _line[1001];
			char * line;
			Uint32 start = SDL_GetTicks();
			int i = 0;
			while (fgets(_line, 1001, file)) {
				line = _line;
				while (line[0] == ' ') {
					line++;
				}
				if (line[0] == 'v') {
					line++;
					if (line[0] == ' ') {
						vertices.push_back(readVec<3>(line));
					}
					else if (line[0] == 'n' && line[1] == ' ') {
						line++;
						normals.push_back(readVec<3>(line));
					}
					else if (line[0] == 't' && line[1] == ' ') {
						line++;
						texcoords.push_back(readVec<3>(line));
					}
				}
				else if (line[0] == 'o' && line[1] == ' ' && vertices.size() > 0) {
					objectID++;
					renderObjects.push_back(RenderObject{ VertexArrayObject(format), Transform(), Transform(), RenderContext() });
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
						Vertex v1(format);
						Vertex v2(format);
						Vertex v3(format);
						if (face[0][1] != INT_MAX && face[0][2] == INT_MAX) {
							std::array<double, 15> data = getVertexUVsArray<3>(vertices, texcoords, face);
							v1.setAttribute<double>(format.position_index, { data.begin() + 0,  data.begin() + 3 });
							v2.setAttribute<double>(format.position_index, { data.begin() + 5,  data.begin() + 8 });
							v3.setAttribute<double>(format.position_index, { data.begin() + 10, data.begin() + 13 });
							if (format.uv_index != (size_t)-1) {
								v1.setAttribute<double>(format.uv_index, { data.begin() + 3,  data.begin() + 5 });
								v2.setAttribute<double>(format.uv_index, { data.begin() + 8,  data.begin() + 10 });
								v3.setAttribute<double>(format.uv_index, { data.begin() + 13, data.begin() + 15 });
							}
						}
						else if (face[0][1] == INT_MAX && face[0][2] != INT_MAX) {
							std::array<double, 18> data = getVertexNormalArray<3>(vertices, normals, face);
							v1.setAttribute<double>(format.position_index, { data.begin() + 0,  data.begin() + 3 });
							v2.setAttribute<double>(format.position_index, { data.begin() + 6,  data.begin() + 9 });
							v3.setAttribute<double>(format.position_index, { data.begin() + 12, data.begin() + 15 });
							if (format.normal_index != (size_t)-1) {
								v1.setAttribute<double>(format.normal_index, { data.begin() + 3,  data.begin() + 6 });
								v2.setAttribute<double>(format.normal_index, { data.begin() + 9,  data.begin() + 12 });
								v3.setAttribute<double>(format.normal_index, { data.begin() + 15, data.begin() + 18 });
							}
						}
						else if (face[0][1] != INT_MAX && face[0][2] != INT_MAX) {
							std::array<double, 24> data = getVertexNormalsUVsArray<3>(vertices, normals, texcoords, face);
							v1.setAttribute<double>(format.position_index, { data.begin() + 0,  data.begin() + 3 });
							v2.setAttribute<double>(format.position_index, { data.begin() + 8,  data.begin() + 11 });
							v3.setAttribute<double>(format.position_index, { data.begin() + 16, data.begin() + 19 });
							if (format.normal_index != (size_t)-1) {
								v1.setAttribute<double>(format.normal_index, { data.begin() + 3,  data.begin() + 6 });
								v2.setAttribute<double>(format.normal_index, { data.begin() + 11, data.begin() + 14 });
								v3.setAttribute<double>(format.normal_index, { data.begin() + 19, data.begin() + 21 });
							}
							if (format.uv_index != (size_t)-1) {
								v1.setAttribute<double>(format.uv_index, { data.begin() + 6,  data.begin() + 8 });
								v2.setAttribute<double>(format.uv_index, { data.begin() + 14, data.begin() + 16 });
								v3.setAttribute<double>(format.uv_index, { data.begin() + 21, data.begin() + 24 });
							}
						}
						else {
							std::array<double, 9> data = getVertexArray<3>(vertices, face);
							v1.setAttribute<double>(format.position_index, { data.begin() + 0, data.begin() + 3 });
							v2.setAttribute<double>(format.position_index, { data.begin() + 3, data.begin() + 6 });
							v3.setAttribute<double>(format.position_index, { data.begin() + 6, data.begin() + 9 });
						}
						renderObjects[objectID].VAO.addTriangle(v1, v2, v3);
					}
					else if (face.size() == 4) {
						Vertex v1(format);
						Vertex v2(format);
						Vertex v3(format);
						Vertex v4(format);
						if (face[0][1] != INT_MAX && face[0][2] == INT_MAX) {
							std::array<double, 20> data = getVertexUVsArray<4>(vertices, texcoords, face);
							v1.setAttribute<double>(format.position_index, { data.begin() + 0,  data.begin() + 3 });
							v2.setAttribute<double>(format.position_index, { data.begin() + 5,  data.begin() + 8 });
							v3.setAttribute<double>(format.position_index, { data.begin() + 10, data.begin() + 13 });
							v4.setAttribute<double>(format.position_index, { data.begin() + 15, data.begin() + 18 });
							if (format.uv_index != (size_t)-1) {
								v1.setAttribute<double>(format.uv_index, { data.begin() + 3,  data.begin() + 5 });
								v2.setAttribute<double>(format.uv_index, { data.begin() + 8,  data.begin() + 10 });
								v3.setAttribute<double>(format.uv_index, { data.begin() + 13, data.begin() + 15 });
								v4.setAttribute<double>(format.uv_index, { data.begin() + 18, data.begin() + 20 });
							}
						}
						else if (face[0][1] == INT_MAX && face[0][2] != INT_MAX) {
							std::array<double, 24> data = getVertexNormalArray<4>(vertices, normals, face);
							v1.setAttribute<double>(format.position_index, { data.begin() + 0,  data.begin() + 3 });
							v2.setAttribute<double>(format.position_index, { data.begin() + 6,  data.begin() + 9 });
							v3.setAttribute<double>(format.position_index, { data.begin() + 12, data.begin() + 15 });
							v4.setAttribute<double>(format.position_index, { data.begin() + 18, data.begin() + 21 });
							if (format.normal_index != (size_t)-1) {
								v1.setAttribute<double>(format.normal_index, { data.begin() + 3,  data.begin() + 6 });
								v2.setAttribute<double>(format.normal_index, { data.begin() + 9,  data.begin() + 12 });
								v3.setAttribute<double>(format.normal_index, { data.begin() + 15, data.begin() + 18 });
								v4.setAttribute<double>(format.normal_index, { data.begin() + 15, data.begin() + 18 });
							}
						}
						else if (face[0][1] != INT_MAX && face[0][2] != INT_MAX) {
							std::array<double, 32> data = getVertexNormalsUVsArray<4>(vertices, normals, texcoords, face);
							v1.setAttribute<double>(format.position_index, { data.begin() + 0,  data.begin() + 3 });
							v2.setAttribute<double>(format.position_index, { data.begin() + 8,  data.begin() + 11 });
							v3.setAttribute<double>(format.position_index, { data.begin() + 16, data.begin() + 19 });
							v4.setAttribute<double>(format.position_index, { data.begin() + 24, data.begin() + 27 });
							if (format.normal_index != (size_t)-1) {
								v1.setAttribute<double>(format.normal_index, { data.begin() + 3,  data.begin() + 6 });
								v2.setAttribute<double>(format.normal_index, { data.begin() + 11, data.begin() + 14 });
								v3.setAttribute<double>(format.normal_index, { data.begin() + 19, data.begin() + 22 });
								v4.setAttribute<double>(format.normal_index, { data.begin() + 27, data.begin() + 30 });
							}
							if (format.uv_index != (size_t)-1) {
								v1.setAttribute<double>(format.uv_index, { data.begin() + 6,  data.begin() + 8 });
								v2.setAttribute<double>(format.uv_index, { data.begin() + 14, data.begin() + 16 });
								v3.setAttribute<double>(format.uv_index, { data.begin() + 22, data.begin() + 24 });
								v4.setAttribute<double>(format.uv_index, { data.begin() + 30, data.begin() + 32 });
							}
						}
						else {
							std::array<double, 12> data = getVertexArray<4>(vertices, face);
							v1.setAttribute<double>(format.position_index, { data.begin() + 0, data.begin() + 3 });
							v2.setAttribute<double>(format.position_index, { data.begin() + 3, data.begin() + 6 });
							v3.setAttribute<double>(format.position_index, { data.begin() + 6, data.begin() + 9 });
							v4.setAttribute<double>(format.position_index, { data.begin() + 9, data.begin() + 12 });
						}
						renderObjects[objectID].VAO.addQuad(v1, v2, v3, v4);
					}
					else {
						std::vector<Vertex> verts;
						if (face[0][1] != INT_MAX && face[0][2] == INT_MAX) {
							std::vector<std::array<double, 5>> data = getVertexUVsArray(face.size(), vertices, texcoords, face);
							for (const std::array<double, 5> &array : data) {
								verts.push_back(format);
								verts.back().setAttribute<double>(format.position_index, { array.begin(), array.begin() + 3 });
								if (format.uv_index != (size_t)-1) {
									verts.back().setAttribute<double>(format.uv_index, { array.begin() + 3, array.begin() + 5 });
								}
							}
						}
						else if (face[0][1] == INT_MAX && face[0][2] != INT_MAX) {
							std::vector<std::array<double, 6>> data = getVertexNormalArray(face.size(), vertices, normals, face);
							for (const std::array<double, 6> &array : data) {
								verts.push_back(format);
								verts.back().setAttribute<double>(format.position_index, { array.begin(), array.begin() + 3 });
								if (format.normal_index != (size_t)-1) {
									verts.back().setAttribute<double>(format.normal_index, { array.begin() + 3, array.begin() + 6 });
								}
							}
						}
						else if (face[0][1] != INT_MAX && face[0][2] != INT_MAX) {
							std::vector<std::array<double, 8>> data = getVertexNormalsUVsArray(face.size(), vertices, normals, texcoords, face);
							for (const std::array<double, 8> &array : data) {
								verts.push_back(format);
								verts.back().setAttribute<double>(format.position_index, { array.begin(), array.begin() + 3 });
								if (format.normal_index != (size_t)-1) {
									verts.back().setAttribute<double>(format.normal_index, { array.begin() + 3, array.begin() + 6 });
								}
								if (format.uv_index != (size_t)-1) {
									verts.back().setAttribute<double>(format.uv_index, { array.begin() + 6, array.begin() + 8 });
								}
							}
						}
						else {
							std::vector<std::array<double, 3>> data = getVertexArray(face.size(), vertices, face);
							for (const std::array<double, 3> &array : data) {
								verts.push_back(format);
								verts.back().setAttribute<double>(format.position_index, { array.begin(), array.begin() + 3 });
							}
						}
						renderObjects[objectID].VAO.addPolygon(verts);
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

}
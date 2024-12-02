#include <map>
#include <functional>

#include "Uniform.h"

namespace PiE {

	void Uniform::init(std::string name, GLuint shaderID, Shader_Type type, int count) {
		this->location = glGetUniformLocation(shaderID, name.c_str());
		this->type = type;
		this->count = count;

		switch (type) {
		case INT:
		case VEC2I:
		case VEC3I:
		case VEC4I:
			data.i = (GLint *)malloc(sizeof(GLint) * ((int)type - (int)INT + 1) * count);
			break;
		case FLOAT:
		case VEC2:
		case VEC3:
		case VEC4:
			data.f = (GLfloat *)malloc(sizeof(GLfloat) * ((int)type - (int)FLOAT + 1) * count);
			break;
		case DOUBLE:
		case VEC2D:
		case VEC3D:
		case VEC4D:
			data.d = (GLdouble *)malloc(sizeof(GLdouble) * ((int)type - (int)DOUBLE + 1) * count);
			break;
		case MAT2:
			data.f = (GLfloat *)malloc(sizeof(GLfloat) * 4);
			break;
		case MAT3:
			data.f = (GLfloat *)malloc(sizeof(GLfloat) * 9);
			break;
		case MAT4:
			data.f = (GLfloat *)malloc(sizeof(GLfloat) * 16);
			break;
		case MAT2X3:
		case MAT3X2:
			data.f = (GLfloat *)malloc(sizeof(GLfloat) * 6);
			break;
		case MAT2X4:
		case MAT4X2:
			data.f = (GLfloat *)malloc(sizeof(GLfloat) * 8);
			break;
		case MAT3X4:
		case MAT4X3:
			data.f = (GLfloat *)malloc(sizeof(GLfloat) * 12);
			break;
		case MAT2D:
			data.d = (GLdouble *)malloc(sizeof(GLdouble) * 4);
			break;
		case MAT3D:
			data.d = (GLdouble *)malloc(sizeof(GLdouble) * 9);
			break;
		case MAT4D:
			data.d = (GLdouble *)malloc(sizeof(GLdouble) * 16);
			break;
		case MAT2X3D:
		case MAT3X2D:
			data.d = (GLdouble *)malloc(sizeof(GLdouble) * 6);
			break;
		case MAT2X4D:
		case MAT4X2D:
			data.d = (GLdouble *)malloc(sizeof(GLfloat) * 8);
			break;
		case MAT3X4D:
		case MAT4X3D:
			data.d = (GLdouble *)malloc(sizeof(GLfloat) * 12);
			break;
		default:
			data.v = NULL;
		}

	}

	void Uniform::destroy() {
		free(data.v);
		data.v = NULL;
	}

	#define EXPAND1(type, data) data.type[0]
	#define EXPAND2(type, data) data.type[0], data.type[1]
	#define EXPAND3(type, data) data.type[0], data.type[1], data.type[2]
	#define EXPAND4(type, data) data.type[0], data.type[1], data.type[2], data.type[3]

	#define EXPAND(type, data, size) EXPAND##size(type, data)

	#define glUniform(size, type, location, data, count) glUniform##size##type##v(location, count, data.##type)

	std::map<Shader_Type, std::function<void(Uniform &)>> uniformApplyMap{
		{INT, [](Uniform & uniform) { glUniform(1, i, uniform.location, uniform.data, uniform.count); } },
		{VEC2I, [](Uniform & uniform) { glUniform(2, i, uniform.location, uniform.data, uniform.count); } },
		{VEC3I, [](Uniform & uniform) { glUniform(3, i, uniform.location, uniform.data, uniform.count); } },
		{VEC4I, [](Uniform & uniform) { glUniform(4, i, uniform.location, uniform.data, uniform.count); } },
		{FLOAT , [](Uniform & uniform) { glUniform(1, f, uniform.location, uniform.data, uniform.count); } },
		{VEC2 , [](Uniform & uniform) { glUniform(2, f, uniform.location, uniform.data, uniform.count); } },
		{VEC3 , [](Uniform & uniform) { glUniform(3, f, uniform.location, uniform.data, uniform.count); } },
		{VEC4 , [](Uniform & uniform) { glUniform(4, f, uniform.location, uniform.data, uniform.count); } },
		{DOUBLE, [](Uniform & uniform) { glUniform(1, d, uniform.location, uniform.data, uniform.count); } },
		{VEC2D, [](Uniform & uniform) { glUniform(2, d, uniform.location, uniform.data, uniform.count); } },
		{VEC3D, [](Uniform & uniform) { glUniform(3, d, uniform.location, uniform.data, uniform.count); } },
		{VEC4D, [](Uniform & uniform) { glUniform(4, d, uniform.location, uniform.data, uniform.count); } },
		{MAT2 , [](Uniform & uniform) { glUniformMatrix2fv(uniform.location, 1, false, uniform.data.f); } },
		{MAT3 , [](Uniform & uniform) { glUniformMatrix3fv(uniform.location, 1, false, uniform.data.f); } },
		{MAT4 , [](Uniform & uniform) { glUniformMatrix4fv(uniform.location, 1, false, uniform.data.f); } },
		{MAT2X3 , [](Uniform & uniform) { glUniformMatrix2x3fv(uniform.location, 1, false, uniform.data.f); } },
		{MAT3X2 , [](Uniform & uniform) { glUniformMatrix3x2fv(uniform.location, 1, false, uniform.data.f); } },
		{MAT2X4 , [](Uniform & uniform) { glUniformMatrix2x4fv(uniform.location, 1, false, uniform.data.f); } },
		{MAT4X2 , [](Uniform & uniform) { glUniformMatrix4x2fv(uniform.location, 1, false, uniform.data.f); } },
		{MAT3X4 , [](Uniform & uniform) { glUniformMatrix3x4fv(uniform.location, 1, false, uniform.data.f); } },
		{MAT4X3 , [](Uniform & uniform) { glUniformMatrix4x3fv(uniform.location, 1, false, uniform.data.f); } },
		{MAT2D, [](Uniform & uniform) { glUniformMatrix2dv(uniform.location, 1, false, uniform.data.d); } },
		{MAT3D, [](Uniform & uniform) { glUniformMatrix3dv(uniform.location, 1, false, uniform.data.d); } },
		{MAT4D, [](Uniform & uniform) { glUniformMatrix4dv(uniform.location, 1, false, uniform.data.d); } },
		{MAT2X3D, [](Uniform & uniform) { glUniformMatrix2x3dv(uniform.location, 1, false, uniform.data.d); } },
		{MAT3X2D, [](Uniform & uniform) { glUniformMatrix3x2dv(uniform.location, 1, false, uniform.data.d); } },
		{MAT2X4D , [](Uniform & uniform) { glUniformMatrix2x4dv(uniform.location, 1, false, uniform.data.d); } },
		{MAT4X2D , [](Uniform & uniform) { glUniformMatrix4x2dv(uniform.location, 1, false, uniform.data.d); } },
		{MAT3X4D , [](Uniform & uniform) { glUniformMatrix3x4dv(uniform.location, 1, false, uniform.data.d); } },
		{MAT4X3D , [](Uniform & uniform) { glUniformMatrix4x3dv(uniform.location, 1, false, uniform.data.d); } },
	};

	void Uniform::apply() {
		uniformApplyMap[type](*this);
	}

}
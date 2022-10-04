#include <map>
#include <functional>

#include "Uniform.h"

void Uniform::init(std::string name, GLuint shaderID, Uniform_Type type, int count) {
	this->location = glGetUniformLocation(shaderID, name.c_str());
	this->type = type;
	this->count = count;

	switch (type)
	{
	case Uniform_Type::INT:
	case Uniform_Type::VEC2I:
	case Uniform_Type::VEC3I:
	case Uniform_Type::VEC4I:
		data.i = (GLint*) malloc(sizeof(GLint) * ((int)type - (int)Uniform_Type::INT + 1) * count);
		break;
	case Uniform_Type::FLOAT:
	case Uniform_Type::VEC2:
	case Uniform_Type::VEC3:
	case Uniform_Type::VEC4:
		data.f = (GLfloat*) malloc(sizeof(GLfloat) * ((int)type - (int)Uniform_Type::FLOAT + 1) * count);
		break;
	case Uniform_Type::DOUBLE:
	case Uniform_Type::VEC2D:
	case Uniform_Type::VEC3D:
	case Uniform_Type::VEC4D:
		data.d = (GLdouble*) malloc(sizeof(GLdouble) * ((int)type - (int)Uniform_Type::DOUBLE + 1) * count);
		break;
	case Uniform_Type::MAT2:
		data.f = (GLfloat*) malloc(sizeof(GLfloat) * 4);
		break;
	case Uniform_Type::MAT3:
		data.f = (GLfloat*) malloc(sizeof(GLfloat) * 9);
		break;
	case Uniform_Type::MAT4:
		data.f = (GLfloat*) malloc(sizeof(GLfloat) * 16);
		break;
	case Uniform_Type::MAT2X3:
	case Uniform_Type::MAT3X2:
		data.f = (GLfloat*) malloc(sizeof(GLfloat) * 6);
		break;
	case Uniform_Type::MAT2X4:
	case Uniform_Type::MAT4X2:
		data.f = (GLfloat*) malloc(sizeof(GLfloat) * 8);
		break;
	case Uniform_Type::MAT3X4:
	case Uniform_Type::MAT4X3:
		data.f = (GLfloat*) malloc(sizeof(GLfloat) * 12);
		break;
	case Uniform_Type::MAT2D:
		data.d = (GLdouble*) malloc(sizeof(GLdouble) * 4);
		break;
	case Uniform_Type::MAT3D:
		data.d = (GLdouble*) malloc(sizeof(GLdouble) * 9);
		break;
	case Uniform_Type::MAT4D:
		data.d = (GLdouble*) malloc(sizeof(GLdouble) * 16);
		break;
	case Uniform_Type::MAT2X3D:
	case Uniform_Type::MAT3X2D:
		data.d = (GLdouble*) malloc(sizeof(GLdouble) * 6);
		break;
	case Uniform_Type::MAT2X4D:
	case Uniform_Type::MAT4X2D:
		data.d = (GLdouble*) malloc(sizeof(GLfloat) * 8);
		break;
	case Uniform_Type::MAT3X4D:
	case Uniform_Type::MAT4X3D:
		data.d = (GLdouble*) malloc(sizeof(GLfloat) * 12);
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

std::map<Uniform_Type, std::function<void(Uniform&)>> uniformApplyMap{
	{Uniform_Type::INT, [](Uniform &uniform) { glUniform(1, i, uniform.location, uniform.data, uniform.count); } },
	{Uniform_Type::VEC2I, [](Uniform &uniform) { glUniform(2, i, uniform.location, uniform.data, uniform.count); } },
	{Uniform_Type::VEC3I, [](Uniform &uniform) { glUniform(3, i, uniform.location, uniform.data, uniform.count); } },
	{Uniform_Type::VEC4I, [](Uniform &uniform) { glUniform(4, i, uniform.location, uniform.data, uniform.count); } },
	{Uniform_Type::FLOAT , [](Uniform &uniform) { glUniform(1, f, uniform.location, uniform.data, uniform.count); } },
	{Uniform_Type::VEC2 , [](Uniform &uniform) { glUniform(2, f, uniform.location, uniform.data, uniform.count); } },
	{Uniform_Type::VEC3 , [](Uniform &uniform) { glUniform(3, f, uniform.location, uniform.data, uniform.count); } },
	{Uniform_Type::VEC4 , [](Uniform &uniform) { glUniform(4, f, uniform.location, uniform.data, uniform.count); } },
	{Uniform_Type::DOUBLE, [](Uniform &uniform) { glUniform(1, d, uniform.location, uniform.data, uniform.count); } },
	{Uniform_Type::VEC2D, [](Uniform &uniform) { glUniform(2, d, uniform.location, uniform.data, uniform.count); } },
	{Uniform_Type::VEC3D, [](Uniform &uniform) { glUniform(3, d, uniform.location, uniform.data, uniform.count); } },
	{Uniform_Type::VEC4D, [](Uniform &uniform) { glUniform(4, d, uniform.location, uniform.data, uniform.count); } },
	{Uniform_Type::MAT2 , [](Uniform &uniform) { glUniformMatrix2fv(uniform.location, 1, false, uniform.data.f); } },
	{Uniform_Type::MAT3 , [](Uniform &uniform) { glUniformMatrix3fv(uniform.location, 1, false, uniform.data.f); } },
	{Uniform_Type::MAT4 , [](Uniform &uniform) { glUniformMatrix4fv(uniform.location, 1, false, uniform.data.f); } },
	{Uniform_Type::MAT2X3 , [](Uniform &uniform) { glUniformMatrix2x3fv(uniform.location, 1, false, uniform.data.f); } },
	{Uniform_Type::MAT3X2 , [](Uniform &uniform) { glUniformMatrix3x2fv(uniform.location, 1, false, uniform.data.f); } },
	{Uniform_Type::MAT2X4 , [](Uniform &uniform) { glUniformMatrix2x4fv(uniform.location, 1, false, uniform.data.f); } },
	{Uniform_Type::MAT4X2 , [](Uniform &uniform) { glUniformMatrix4x2fv(uniform.location, 1, false, uniform.data.f); } },
	{Uniform_Type::MAT3X4 , [](Uniform &uniform) { glUniformMatrix3x4fv(uniform.location, 1, false, uniform.data.f); } },
	{Uniform_Type::MAT4X3 , [](Uniform &uniform) { glUniformMatrix4x3fv(uniform.location, 1, false, uniform.data.f); } },
	{Uniform_Type::MAT2D, [](Uniform &uniform) { glUniformMatrix2dv(uniform.location, 1, false, uniform.data.d); } },
	{Uniform_Type::MAT3D, [](Uniform &uniform) { glUniformMatrix3dv(uniform.location, 1, false, uniform.data.d); } },
	{Uniform_Type::MAT4D, [](Uniform &uniform) { glUniformMatrix4dv(uniform.location, 1, false, uniform.data.d); } },
	{Uniform_Type::MAT2X3D, [](Uniform &uniform) { glUniformMatrix2x3dv(uniform.location, 1, false, uniform.data.d); } },
	{Uniform_Type::MAT3X2D, [](Uniform &uniform) { glUniformMatrix3x2dv(uniform.location, 1, false, uniform.data.d); } },
	{Uniform_Type::MAT2X4D , [](Uniform &uniform) { glUniformMatrix2x4dv(uniform.location, 1, false, uniform.data.d); } },
	{Uniform_Type::MAT4X2D , [](Uniform &uniform) { glUniformMatrix4x2dv(uniform.location, 1, false, uniform.data.d); } },
	{Uniform_Type::MAT3X4D , [](Uniform &uniform) { glUniformMatrix3x4dv(uniform.location, 1, false, uniform.data.d); } },
	{Uniform_Type::MAT4X3D , [](Uniform &uniform) { glUniformMatrix4x3dv(uniform.location, 1, false, uniform.data.d); } },
};

void Uniform::apply() {
	uniformApplyMap[type](*this);
}
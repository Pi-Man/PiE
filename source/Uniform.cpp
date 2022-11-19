#include <map>
#include <functional>

#include "Uniform.h"

void Uniform::init(std::string name, GLuint shaderID, ShaderTypes::Shader_Type type, int count) {
	this->location = glGetUniformLocation(shaderID, name.c_str());
	this->type = type;
	this->count = count;

	switch (type)
	{
	case ShaderTypes::INT:
	case ShaderTypes::VEC2I:
	case ShaderTypes::VEC3I:
	case ShaderTypes::VEC4I:
		data.i = (GLint*) malloc(sizeof(GLint) * ((int)type - (int)ShaderTypes::INT + 1) * count);
		break;
	case ShaderTypes::FLOAT:
	case ShaderTypes::VEC2:
	case ShaderTypes::VEC3:
	case ShaderTypes::VEC4:
		data.f = (GLfloat*) malloc(sizeof(GLfloat) * ((int)type - (int)ShaderTypes::FLOAT + 1) * count);
		break;
	case ShaderTypes::DOUBLE:
	case ShaderTypes::VEC2D:
	case ShaderTypes::VEC3D:
	case ShaderTypes::VEC4D:
		data.d = (GLdouble*) malloc(sizeof(GLdouble) * ((int)type - (int)ShaderTypes::DOUBLE + 1) * count);
		break;
	case ShaderTypes::MAT2:
		data.f = (GLfloat*) malloc(sizeof(GLfloat) * 4);
		break;
	case ShaderTypes::MAT3:
		data.f = (GLfloat*) malloc(sizeof(GLfloat) * 9);
		break;
	case ShaderTypes::MAT4:
		data.f = (GLfloat*) malloc(sizeof(GLfloat) * 16);
		break;
	case ShaderTypes::MAT2X3:
	case ShaderTypes::MAT3X2:
		data.f = (GLfloat*) malloc(sizeof(GLfloat) * 6);
		break;
	case ShaderTypes::MAT2X4:
	case ShaderTypes::MAT4X2:
		data.f = (GLfloat*) malloc(sizeof(GLfloat) * 8);
		break;
	case ShaderTypes::MAT3X4:
	case ShaderTypes::MAT4X3:
		data.f = (GLfloat*) malloc(sizeof(GLfloat) * 12);
		break;
	case ShaderTypes::MAT2D:
		data.d = (GLdouble*) malloc(sizeof(GLdouble) * 4);
		break;
	case ShaderTypes::MAT3D:
		data.d = (GLdouble*) malloc(sizeof(GLdouble) * 9);
		break;
	case ShaderTypes::MAT4D:
		data.d = (GLdouble*) malloc(sizeof(GLdouble) * 16);
		break;
	case ShaderTypes::MAT2X3D:
	case ShaderTypes::MAT3X2D:
		data.d = (GLdouble*) malloc(sizeof(GLdouble) * 6);
		break;
	case ShaderTypes::MAT2X4D:
	case ShaderTypes::MAT4X2D:
		data.d = (GLdouble*) malloc(sizeof(GLfloat) * 8);
		break;
	case ShaderTypes::MAT3X4D:
	case ShaderTypes::MAT4X3D:
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

std::map<ShaderTypes::Shader_Type, std::function<void(Uniform&)>> uniformApplyMap{
	{ShaderTypes::INT, [](Uniform &uniform) { glUniform(1, i, uniform.location, uniform.data, uniform.count); } },
	{ShaderTypes::VEC2I, [](Uniform &uniform) { glUniform(2, i, uniform.location, uniform.data, uniform.count); } },
	{ShaderTypes::VEC3I, [](Uniform &uniform) { glUniform(3, i, uniform.location, uniform.data, uniform.count); } },
	{ShaderTypes::VEC4I, [](Uniform &uniform) { glUniform(4, i, uniform.location, uniform.data, uniform.count); } },
	{ShaderTypes::FLOAT , [](Uniform &uniform) { glUniform(1, f, uniform.location, uniform.data, uniform.count); } },
	{ShaderTypes::VEC2 , [](Uniform &uniform) { glUniform(2, f, uniform.location, uniform.data, uniform.count); } },
	{ShaderTypes::VEC3 , [](Uniform &uniform) { glUniform(3, f, uniform.location, uniform.data, uniform.count); } },
	{ShaderTypes::VEC4 , [](Uniform &uniform) { glUniform(4, f, uniform.location, uniform.data, uniform.count); } },
	{ShaderTypes::DOUBLE, [](Uniform &uniform) { glUniform(1, d, uniform.location, uniform.data, uniform.count); } },
	{ShaderTypes::VEC2D, [](Uniform &uniform) { glUniform(2, d, uniform.location, uniform.data, uniform.count); } },
	{ShaderTypes::VEC3D, [](Uniform &uniform) { glUniform(3, d, uniform.location, uniform.data, uniform.count); } },
	{ShaderTypes::VEC4D, [](Uniform &uniform) { glUniform(4, d, uniform.location, uniform.data, uniform.count); } },
	{ShaderTypes::MAT2 , [](Uniform &uniform) { glUniformMatrix2fv(uniform.location, 1, false, uniform.data.f); } },
	{ShaderTypes::MAT3 , [](Uniform &uniform) { glUniformMatrix3fv(uniform.location, 1, false, uniform.data.f); } },
	{ShaderTypes::MAT4 , [](Uniform &uniform) { glUniformMatrix4fv(uniform.location, 1, false, uniform.data.f); } },
	{ShaderTypes::MAT2X3 , [](Uniform &uniform) { glUniformMatrix2x3fv(uniform.location, 1, false, uniform.data.f); } },
	{ShaderTypes::MAT3X2 , [](Uniform &uniform) { glUniformMatrix3x2fv(uniform.location, 1, false, uniform.data.f); } },
	{ShaderTypes::MAT2X4 , [](Uniform &uniform) { glUniformMatrix2x4fv(uniform.location, 1, false, uniform.data.f); } },
	{ShaderTypes::MAT4X2 , [](Uniform &uniform) { glUniformMatrix4x2fv(uniform.location, 1, false, uniform.data.f); } },
	{ShaderTypes::MAT3X4 , [](Uniform &uniform) { glUniformMatrix3x4fv(uniform.location, 1, false, uniform.data.f); } },
	{ShaderTypes::MAT4X3 , [](Uniform &uniform) { glUniformMatrix4x3fv(uniform.location, 1, false, uniform.data.f); } },
	{ShaderTypes::MAT2D, [](Uniform &uniform) { glUniformMatrix2dv(uniform.location, 1, false, uniform.data.d); } },
	{ShaderTypes::MAT3D, [](Uniform &uniform) { glUniformMatrix3dv(uniform.location, 1, false, uniform.data.d); } },
	{ShaderTypes::MAT4D, [](Uniform &uniform) { glUniformMatrix4dv(uniform.location, 1, false, uniform.data.d); } },
	{ShaderTypes::MAT2X3D, [](Uniform &uniform) { glUniformMatrix2x3dv(uniform.location, 1, false, uniform.data.d); } },
	{ShaderTypes::MAT3X2D, [](Uniform &uniform) { glUniformMatrix3x2dv(uniform.location, 1, false, uniform.data.d); } },
	{ShaderTypes::MAT2X4D , [](Uniform &uniform) { glUniformMatrix2x4dv(uniform.location, 1, false, uniform.data.d); } },
	{ShaderTypes::MAT4X2D , [](Uniform &uniform) { glUniformMatrix4x2dv(uniform.location, 1, false, uniform.data.d); } },
	{ShaderTypes::MAT3X4D , [](Uniform &uniform) { glUniformMatrix3x4dv(uniform.location, 1, false, uniform.data.d); } },
	{ShaderTypes::MAT4X3D , [](Uniform &uniform) { glUniformMatrix4x3dv(uniform.location, 1, false, uniform.data.d); } },
};

void Uniform::apply() {
	uniformApplyMap[type](*this);
}
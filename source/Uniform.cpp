#include <map>
#include <functional>

#include "Uniform.h"

void Uniform::init(std::string name, GLuint shaderID, Uniform_Type type, int count) {
	this->location = glGetUniformLocation(shaderID, name.c_str());
	this->type = type;
	this->count = count;

	switch (type)
	{
	case VEC1I:
	case VEC2I:
	case VEC3I:
	case VEC4I:
		data.v = malloc(sizeof(GLint) * (type - VEC1I + 1) * count);
		break;
	case VEC1:
	case VEC2:
	case VEC3:
	case VEC4:
		data.v = malloc(sizeof(GLfloat) * (type - VEC1 + 1) * count);
		break;
	case VEC1D:
	case VEC2D:
	case VEC3D:
	case VEC4D:
		data.v = malloc(sizeof(GLdouble) * (type - VEC1D + 1) * count);
		break;
	case MAT4:
		data.v = malloc(sizeof(GLfloat) * 16);
		break;
	case MAT4D:
		data.v = malloc(sizeof(GLdouble) * 16);
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
	{VEC1I, [](Uniform &uniform) { glUniform(1, i, uniform.location, uniform.data, uniform.count); } },
	{VEC2I, [](Uniform &uniform) { glUniform(2, i, uniform.location, uniform.data, uniform.count); } },
	{VEC3I, [](Uniform &uniform) { glUniform(3, i, uniform.location, uniform.data, uniform.count); } },
	{VEC4I, [](Uniform &uniform) { glUniform(4, i, uniform.location, uniform.data, uniform.count); } },
	{VEC1 , [](Uniform &uniform) { glUniform(1, f, uniform.location, uniform.data, uniform.count); } },
	{VEC2 , [](Uniform &uniform) { glUniform(2, f, uniform.location, uniform.data, uniform.count); } },
	{VEC3 , [](Uniform &uniform) { glUniform(3, f, uniform.location, uniform.data, uniform.count); } },
	{VEC4 , [](Uniform &uniform) { glUniform(4, f, uniform.location, uniform.data, uniform.count); } },
	{VEC1D, [](Uniform &uniform) { glUniform(1, d, uniform.location, uniform.data, uniform.count); } },
	{VEC2D, [](Uniform &uniform) { glUniform(2, d, uniform.location, uniform.data, uniform.count); } },
	{VEC3D, [](Uniform &uniform) { glUniform(3, d, uniform.location, uniform.data, uniform.count); } },
	{VEC4D, [](Uniform &uniform) { glUniform(4, d, uniform.location, uniform.data, uniform.count); } },
	{MAT4 , [](Uniform &uniform) { glUniformMatrix4fv(uniform.location, 1, false, uniform.data.f); } },
	{MAT4D, [](Uniform &uniform) { glUniformMatrix4dv(uniform.location, 1, false, uniform.data.d); } },
};

void Uniform::apply() {
	uniformApplyMap[type](*this);
}
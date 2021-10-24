#include <map>
#include <functional>

#include "Uniform.h"

void Uniform::init(std::string name, GLuint shaderID, Uniform_Type type) {
	this->location = glGetUniformLocation(shaderID, name.c_str());
	this->type = type;

	switch (type)
	{
	case VEC1I:
	case VEC2I:
	case VEC3I:
	case VEC4I:
		data.v = malloc(sizeof(GLint) * (type - VEC1I + 1));
		break;
	case VEC1:
	case VEC2:
	case VEC3:
	case VEC4:
		data.v = malloc(sizeof(GLfloat) * (type - VEC1 + 1));
		break;
	case VEC1D:
	case VEC2D:
	case VEC3D:
	case VEC4D:
		data.v = malloc(sizeof(GLdouble) * (type - VEC1D + 1));
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

#define glUniform(size, type, location, data) glUniform##size##type(location, EXPAND(type, data, size))

std::map<Uniform_Type, std::function<void(Uniform&)>> uniformApplyMap{
	{VEC1I, [](Uniform &uniform) { glUniform(1, i, uniform.location, uniform.data); } },
	{VEC2I, [](Uniform &uniform) { glUniform(2, i, uniform.location, uniform.data); } },
	{VEC3I, [](Uniform &uniform) { glUniform(3, i, uniform.location, uniform.data); } },
	{VEC4I, [](Uniform &uniform) { glUniform(4, i, uniform.location, uniform.data); } },
	{VEC1 , [](Uniform &uniform) { glUniform(1, f, uniform.location, uniform.data); } },
	{VEC2 , [](Uniform &uniform) { glUniform(2, f, uniform.location, uniform.data); } },
	{VEC3 , [](Uniform &uniform) { glUniform(3, f, uniform.location, uniform.data); } },
	{VEC4 , [](Uniform &uniform) { glUniform(4, f, uniform.location, uniform.data); } },
	{VEC1D, [](Uniform &uniform) { glUniform(1, d, uniform.location, uniform.data); } },
	{VEC2D, [](Uniform &uniform) { glUniform(2, d, uniform.location, uniform.data); } },
	{VEC3D, [](Uniform &uniform) { glUniform(3, d, uniform.location, uniform.data); } },
	{VEC4D, [](Uniform &uniform) { glUniform(4, d, uniform.location, uniform.data); } },
	{MAT4 , [](Uniform &uniform) { glUniformMatrix4fv(uniform.location, 1, false, uniform.data.f); } },
	{MAT4D, [](Uniform &uniform) { glUniformMatrix4dv(uniform.location, 1, false, uniform.data.d); } },
};

void Uniform::apply() {
	uniformApplyMap[type](*this);
	//switch (type)
	//{
	//case VEC1I:
	//	glUniform(1, i, location, data);
	//	break;
	//case VEC2I:
	//	glUniform(2, i, location, data);
	//	break;
	//case VEC3I:
	//	glUniform(3, i, location, data);
	//	break;
	//case VEC4I:
	//	glUniform(4, i, location, data);
	//	break;
	//case VEC1:
	//	glUniform(1, f, location, data);
	//	break;
	//case VEC2:
	//	glUniform(2, f, location, data);
	//	break;
	//case VEC3:
	//	glUniform(3, f, location, data);
	//	break;
	//case VEC4:
	//	glUniform(4, f, location, data);
	//	break;
	//case VEC1D:
	//	glUniform(1, d, location, data);
	//	break;
	//case VEC2D:
	//	glUniform(2, d, location, data);
	//	break;
	//case VEC3D:
	//	glUniform(3, d, location, data);
	//	break;
	//case VEC4D:
	//	glUniform(4, d, location, data);
	//	break;
	//case MAT4:
	//	glUniformMatrix4fv(location, 1, false, data.f);
	//	break;
	//case MAT4D:
	//	glUniformMatrix4dv(location, 1, false, data.d);
	//	break;
	//default:
	//	break;
	//}
}


#include "Uniform.h"

void Uniform::init(std::string name, GLuint shaderID, Uniform_Type type) {
	this->location = glGetUniformLocation(shaderID, name.c_str());
	this->type = type;

	switch (type)
	{
	case I:
	case VEC2I:
	case VEC3I:
	case VEC4I:
		data.v = malloc(sizeof(GLint) * (type - I + 1));
		break;
	case F:
	case VEC2:
	case VEC3:
	case VEC4:
		data.v = malloc(sizeof(GLfloat) * (type - F + 1));
		break;
	case MAT4:
		data.v = malloc(sizeof(GLfloat) * 16);
		break;
	default:
		data.v = NULL;
	}

}

void Uniform::destroy() {
	free(data.v);
	data.v = NULL;
}

#define CAST1(type, data) ((type*)data)[0]
#define CAST2(type, data) ((type*)data)[0], ((type*)data)[1]
#define CAST3(type, data) ((type*)data)[0], ((type*)data)[1], ((type*)data)[2]
#define CAST4(type, data) ((type*)data)[0], ((type*)data)[1], ((type*)data)[2], ((type*)data)[3]

void Uniform::apply() {
	switch (type)
	{
	case I:
		glUniform1i(location, data.i[0]);
		break;
	case VEC2I:
		glUniform2i(location, data.i[0], data.i[1]);
		break;
	case VEC3I:
		glUniform3i(location, data.i[0], data.i[1], data.i[2]);
		break;
	case VEC4I:
		glUniform4i(location, data.i[0], data.i[1], data.i[2], data.i[3]);
		break;
	case F:
		glUniform1f(location, data.f[0]);
		break;
	case VEC2:
		glUniform2f(location, data.f[0], data.f[1]);
		break;
	case VEC3:
		glUniform3f(location, data.f[0], data.f[1], data.f[2]);
		break;
	case VEC4:
		glUniform4f(location, data.f[0], data.f[1], data.f[2], data.f[3]);
		break;
	case MAT4:
		glUniformMatrix4fv(location, 1, false, data.f);
		break;
	default:
		break;
	}
}
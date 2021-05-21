

#include "Uniform.h"

void Uniform::init(char * name, GLuint shaderID, Uniform_Type type) {
	this->location = glGetUniformLocation(shaderID, name);
	this->type = type;

	switch (type)
	{
	case I:
	case VEC2I:
	case VEC3I:
	case VEC4I:
		data = malloc(sizeof(GLint) * (type - I + 1));
		break;
	case F:
	case VEC2:
	case VEC3:
	case VEC4:
		data = malloc(sizeof(GLfloat) * (type - F + 1));
		break;
	case MAT4:
		data = malloc(sizeof(GLfloat) * 16);
		break;
	default:
		data = NULL;
	}

}

#define CAST1(type, data) ((type*)data)[0]
#define CAST2(type, data) ((type*)data)[0], ((type*)data)[1]
#define CAST3(type, data) ((type*)data)[0], ((type*)data)[1], ((type*)data)[2]
#define CAST4(type, data) ((type*)data)[0], ((type*)data)[1], ((type*)data)[2], ((type*)data)[3]

void Uniform::apply() {
	switch (type)
	{
	case I:
		glUniform1i(location, CAST1(GLint, data));
		break;
	case VEC2I:
		glUniform2i(location, CAST2(GLint, data));
		break;
	case VEC3I:
		glUniform3i(location, CAST3(GLint, data));
		break;
	case VEC4I:
		glUniform4i(location, CAST4(GLint, data));
		break;
	case F:
		glUniform1f(location, CAST1(GLfloat, data));
		break;
	case VEC2:
		glUniform2f(location, CAST2(GLfloat, data));
		break;
	case VEC3:
		glUniform3f(location, CAST3(GLfloat, data));
		break;
	case VEC4:
		glUniform4f(location, CAST4(GLfloat, data));
		break;
	case MAT4:
		glUniformMatrix4fv(location, 1, false, (GLfloat*)data);
		break;
	default:
		break;
	}
}

#ifndef _SHADER_TYPES_H
#define _SHADER_TYPES_H

#include<unordered_map>

#include<glew.h>

namespace PiE {

	enum Shader_Type {
		INT,
		VEC2I,
		VEC3I,
		VEC4I,
		FLOAT,
		VEC2,
		VEC3,
		VEC4,
		DOUBLE,
		VEC2D,
		VEC3D,
		VEC4D,
		MAT2,
		MAT3,
		MAT4,
		MAT2X3,
		MAT3X2,
		MAT2X4,
		MAT4X2,
		MAT3X4,
		MAT4X3,
		MAT2D,
		MAT3D,
		MAT4D,
		MAT2X3D,
		MAT3X2D,
		MAT2X4D,
		MAT4X2D,
		MAT3X4D,
		MAT4X3D
	};

	template<typename T>
	struct gl_type_to_enum {};

	template<>
	struct gl_type_to_enum<GLbyte> {
		enum {
			type = GL_BYTE
		};
	};

	template<>
	struct gl_type_to_enum<GLubyte> {
		enum {
			type = GL_UNSIGNED_BYTE
		};
	};

	template<>
	struct gl_type_to_enum<GLshort> {
		enum {
			type = GL_SHORT
		};
	};

	template<>
	struct gl_type_to_enum<GLushort> {
		enum {
			type = GL_UNSIGNED_SHORT
		};
	};

	template<>
	struct gl_type_to_enum<GLint> {
		enum {
			type = GL_INT
		};
	};

	template<>
	struct gl_type_to_enum<GLuint> {
		enum {
			type = GL_UNSIGNED_INT
		};
	};

	template<>
	struct gl_type_to_enum<GLfloat> {
		enum {
			type = GL_FLOAT
		};
	};

	template<>
	struct gl_type_to_enum<GLdouble> {
		enum {
			type = GL_DOUBLE
		};
	};

	constexpr GLenum container_of(Shader_Type type) {
		switch (type) {
		case INT:
		case VEC2I:
		case VEC3I:
		case VEC4I:
			return GL_INT;
		case FLOAT:
		case VEC2:
		case VEC3:
		case VEC4:
			return GL_FLOAT;
		case DOUBLE:
		case VEC2D:
		case VEC3D:
		case VEC4D:
			return GL_DOUBLE;
		case MAT2:
		case MAT3:
		case MAT4:
		case MAT2X3:
		case MAT3X2:
		case MAT2X4:
		case MAT4X2:
		case MAT3X4:
		case MAT4X3:
			return GL_FLOAT;
		case MAT2D:
		case MAT3D:
		case MAT4D:
		case MAT2X3D:
		case MAT3X2D:
		case MAT2X4D:
		case MAT4X2D:
		case MAT3X4D:
		case MAT4X3D:
			return GL_DOUBLE;
		default:
			return 0;
		}
	}
	constexpr size_t count_of(Shader_Type type) {
		switch (type) {
		case INT:	  return 1;
		case VEC2I:	  return 2;
		case VEC3I:	  return 3;
		case VEC4I:	  return 4;
		case FLOAT:	  return 1;
		case VEC2:	  return 2;
		case VEC3:	  return 3;
		case VEC4:	  return 4;
		case DOUBLE:  return 1;
		case VEC2D:	  return 2;
		case VEC3D:	  return 3;
		case VEC4D:	  return 4;
		case MAT2:	  return 4;
		case MAT3:	  return 9;
		case MAT4:	  return 16;
		case MAT2X3:  return 6;
		case MAT3X2:  return 6;
		case MAT2X4:  return 8;
		case MAT4X2:  return 8;
		case MAT3X4:  return 12;
		case MAT4X3:  return 12;
		case MAT2D:	  return 4;
		case MAT3D:	  return 9;
		case MAT4D:	  return 16;
		case MAT2X3D: return 6;
		case MAT3X2D: return 6;
		case MAT2X4D: return 8;
		case MAT4X2D: return 8;
		case MAT3X4D: return 12;
		case MAT4X3D: return 12;
		default:      return 0;
		}
	}
	constexpr size_t container_size_of(Shader_Type type) {
		switch (type) {
		case INT:
		case VEC2I:
		case VEC3I:
		case VEC4I:
			return sizeof(GLint);
		case FLOAT:
		case VEC2:
		case VEC3:
		case VEC4:
			return sizeof(GLfloat);
		case DOUBLE:
		case VEC2D:
		case VEC3D:
		case VEC4D:
			return sizeof(GLdouble);
		case MAT2:
		case MAT3:
		case MAT4:
		case MAT2X3:
		case MAT3X2:
		case MAT2X4:
		case MAT4X2:
		case MAT3X4:
		case MAT4X3:
			return sizeof(GLfloat);
		case MAT2D:
		case MAT3D:
		case MAT4D:
		case MAT2X3D:
		case MAT3X2D:
		case MAT2X4D:
		case MAT4X2D:
		case MAT3X4D:
		case MAT4X3D:
			return sizeof(GLdouble);
		default:
			return 0;
		}
	}
}

#endif
//#include "ShaderTypes.h"
//
//#include <glew.h>
//
//namespace ShaderTypes {
//	constexpr GLenum container_of(Shader_Type type) {
//		switch (type) {
//		case INT:
//		case VEC2I:
//		case VEC3I:
//		case VEC4I:
//			return GL_INT;
//		case FLOAT:
//		case VEC2:
//		case VEC3:
//		case VEC4:
//			return GL_FLOAT;
//		case DOUBLE:
//		case VEC2D:
//		case VEC3D:
//		case VEC4D:
//			return GL_DOUBLE;
//		case MAT2:
//		case MAT3:
//		case MAT4:
//		case MAT2X3:
//		case MAT3X2:
//		case MAT2X4:
//		case MAT4X2:
//		case MAT3X4:
//		case MAT4X3:
//			return GL_FLOAT;
//		case MAT2D:
//		case MAT3D:
//		case MAT4D:
//		case MAT2X3D:
//		case MAT3X2D:
//		case MAT2X4D:
//		case MAT4X2D:
//		case MAT3X4D:
//		case MAT4X3D:
//			return GL_DOUBLE;
//		}
//	}
//	constexpr size_t count_of(Shader_Type type) {
//		switch (type) {
//		case INT:	  return 1;
//		case VEC2I:	  return 2;
//		case VEC3I:	  return 3;
//		case VEC4I:	  return 4;
//		case FLOAT:	  return 1;
//		case VEC2:	  return 2;
//		case VEC3:	  return 3;
//		case VEC4:	  return 4;
//		case DOUBLE:  return 1;
//		case VEC2D:	  return 2;
//		case VEC3D:	  return 3;
//		case VEC4D:	  return 4;
//		case MAT2:	  return 4;
//		case MAT3:	  return 9;
//		case MAT4:	  return 16;
//		case MAT2X3:  return 6;
//		case MAT3X2:  return 6;
//		case MAT2X4:  return 8;
//		case MAT4X2:  return 8;
//		case MAT3X4:  return 12;
//		case MAT4X3:  return 12;
//		case MAT2D:	  return 4;
//		case MAT3D:	  return 9;
//		case MAT4D:	  return 16;
//		case MAT2X3D: return 6;
//		case MAT3X2D: return 6;
//		case MAT2X4D: return 8;
//		case MAT4X2D: return 8;
//		case MAT3X4D: return 12;
//		case MAT4X3D: return 12;
//		}
//	}
//	constexpr size_t container_size_of(Shader_Type type) {
//		switch (type) {
//		case INT:
//		case VEC2I:
//		case VEC3I:
//		case VEC4I:
//			return sizeof(GLint);
//		case FLOAT:
//		case VEC2:
//		case VEC3:
//		case VEC4:
//			return sizeof(GLfloat);
//		case DOUBLE:
//		case VEC2D:
//		case VEC3D:
//		case VEC4D:
//			return sizeof(GLdouble);
//		case MAT2:
//		case MAT3:
//		case MAT4:
//		case MAT2X3:
//		case MAT3X2:
//		case MAT2X4:
//		case MAT4X2:
//		case MAT3X4:
//		case MAT4X3:
//			return sizeof(GLfloat);
//		case MAT2D:
//		case MAT3D:
//		case MAT4D:
//		case MAT2X3D:
//		case MAT3X2D:
//		case MAT2X4D:
//		case MAT4X2D:
//		case MAT3X4D:
//		case MAT4X3D:
//			return sizeof(GLdouble);
//		}
//	}
//}
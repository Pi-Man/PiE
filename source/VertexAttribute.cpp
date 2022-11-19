#include "VertexAttribute.h"
//
//Attribute::Attribute() {
//	data.v = nullptr;
//}
//
//void Attribute::free() {
//	std::free(data.v);
//	data.v = nullptr;
//}
//
//VertexAttributeType::~VertexAttributeType() {
//	const_cast<Attribute&>(default_value).free();
//}
//
//VertexFormat::VertexFormat(std::initializer_list<std::reference_wrapper<const VertexAttributeType>> attributes_list) : attributes(attributes_list) {
//	stride = 0;
//	for (const VertexAttributeType & attribute : attributes) {
//		stride += attribute.byteLength;
//	}
//}
//
//bool operator==(const VertexAttributeType & left, const VertexAttributeType & right) {
//	return &left == &right;
//}
//
//bool operator!=(const VertexAttributeType & left, const VertexAttributeType & right) {
//	return !(left == right);
//}
//
//bool operator==(const VertexFormat & left, const VertexFormat & right) {
//
//	if (left.attributes.size() != right.attributes.size() || left.stride != right.stride) return false;
//
//	for (size_t i = 0; i < left.attributes.size(); i++) {
//		if (left.attributes[i] != right.attributes[i]) return false;
//	}
//
//	return true;
//}
//
//bool operator!=(const VertexFormat & left, const VertexFormat & right) {
//	return !(left == right);
//}

Vertex::Vertex(const Vertex & other) : format(other.format), data((char*)malloc(other.format.stride)), exists(other.exists) {
	memcpy(data, other.data, format.stride);
}

Vertex::Vertex(const Vertex && other) : format(other.format), data((char*)malloc(other.format.stride)), exists(other.exists) {
	memmove(data, other.data, format.stride);
}

Vertex::~Vertex() {
	free(data);
}

Vertex & Vertex::operator=(const Vertex & other) {
	if (data) free(data);
	format = other.format;
	data = (char*)malloc(format.stride);
	memcpy(data, other.data, format.stride);
	exists = other.exists;
	return *this;
}

Vertex & Vertex::operator=(const Vertex && other) {
	if (data) free(data);
	format = other.format;
	data = (char*)malloc(format.stride);
	memmove(data, other.data, format.stride);
	exists = other.exists;
	return *this;
}

VertexAttributeType::VertexAttributeType(Usage usage, ShaderTypes::Shader_Type type) : usage(usage), type(type), size(ShaderTypes::container_size_of(type) * ShaderTypes::count_of(type)), count(ShaderTypes::count_of(type)) {
	memset(default_data, 0, size);
}

bool VertexAttributeType::operator==(const VertexAttributeType & other) const {
	return usage == other.usage && type == other.type && memcmp(default_data, other.default_data, size) == 0;
}

bool VertexAttributeType::operator!=(const VertexAttributeType & other) const {
	return !(*this == other);
}

VertexFormat::VertexFormat(std::initializer_list<VertexAttributeType> types) : types(types) {
	size_t offset = 0;
	for (size_t i = 0; i < this->types.size(); i++) {
		offsets.push_back(offset);
		offset += this->types[i].size;
		if (this->types[i].usage == Usage::POSITION) {
			position_index = i;
		}
		else if (this->types[i].usage == Usage::NORMAL) {
			normal_index = i;
		}
		else if (this->types[i].usage == Usage::UV) {
			uv_index = i;
		}
	}
	offsets.push_back(offset);
	stride = offset;
}

bool VertexFormat::operator==(const VertexFormat & other) const {
	if (other.types.size() != types.size()) return false;
	for (size_t i = 0; i < types.size(); i++) {
		if (types[i] != other.types[i]) return false;
	}
	return true;
}

bool VertexFormat::operator!=(const VertexFormat & other) const {
	return !(*this == other);
}

namespace VertexUtility {

	std::unordered_map<GLenum, std::unordered_map<GLenum, std::vector<char>(*)(std::vector<char>)>> castMap{
		{GL_BYTE, std::unordered_map<GLenum, std::vector<char>(*)(std::vector<char>)> {
			{GL_BYTE, castMapFunc<GLbyte, GLbyte>},
			{GL_UNSIGNED_BYTE, castMapFunc<GLbyte, GLubyte>},
			{GL_SHORT, castMapFunc<GLbyte, GLshort>},
			{GL_UNSIGNED_SHORT, castMapFunc<GLbyte, GLushort>},
			{GL_INT, castMapFunc<GLbyte, GLint>},
			{GL_UNSIGNED_INT, castMapFunc<GLbyte, GLuint>},
			{GL_FLOAT, castMapFunc<GLbyte, GLfloat>},
			{GL_DOUBLE, castMapFunc<GLbyte, GLdouble>}
		}},
		{GL_UNSIGNED_BYTE, std::unordered_map<GLenum, std::vector<char>(*)(std::vector<char>)> {
			{GL_BYTE, castMapFunc<GLubyte, GLbyte>},
			{GL_UNSIGNED_BYTE, castMapFunc<GLubyte, GLubyte>},
			{GL_SHORT, castMapFunc<GLubyte, GLshort>},
			{GL_UNSIGNED_SHORT, castMapFunc<GLubyte, GLushort>},
			{GL_INT, castMapFunc<GLubyte, GLint>},
			{GL_UNSIGNED_INT, castMapFunc<GLubyte, GLuint>},
			{GL_FLOAT, castMapFunc<GLubyte, GLfloat>},
			{GL_DOUBLE, castMapFunc<GLubyte, GLdouble>}
		}},
		{GL_SHORT, std::unordered_map<GLenum, std::vector<char>(*)(std::vector<char>)> {
			{GL_BYTE, castMapFunc<GLshort, GLbyte>},
			{GL_UNSIGNED_BYTE, castMapFunc<GLshort, GLubyte>},
			{GL_SHORT, castMapFunc<GLshort, GLshort>},
			{GL_UNSIGNED_SHORT, castMapFunc<GLshort, GLushort>},
			{GL_INT, castMapFunc<GLshort, GLint>},
			{GL_UNSIGNED_INT, castMapFunc<GLshort, GLuint>},
			{GL_FLOAT, castMapFunc<GLshort, GLfloat>},
			{GL_DOUBLE, castMapFunc<GLshort, GLdouble>}
		}},
		{GL_UNSIGNED_SHORT, std::unordered_map<GLenum, std::vector<char>(*)(std::vector<char>)> {
			{GL_BYTE, castMapFunc<GLushort, GLbyte>},
			{GL_UNSIGNED_BYTE, castMapFunc<GLushort, GLubyte>},
			{GL_SHORT, castMapFunc<GLushort, GLshort>},
			{GL_UNSIGNED_SHORT, castMapFunc<GLushort, GLushort>},
			{GL_INT, castMapFunc<GLushort, GLint>},
			{GL_UNSIGNED_INT, castMapFunc<GLushort, GLuint>},
			{GL_FLOAT, castMapFunc<GLushort, GLfloat>},
			{GL_DOUBLE, castMapFunc<GLushort, GLdouble>}
		}},
		{GL_INT, std::unordered_map<GLenum, std::vector<char>(*)(std::vector<char>)> {
			{GL_BYTE, castMapFunc<GLint, GLbyte>},
			{GL_UNSIGNED_BYTE, castMapFunc<GLint, GLubyte>},
			{GL_SHORT, castMapFunc<GLint, GLshort>},
			{GL_UNSIGNED_SHORT, castMapFunc<GLint, GLushort>},
			{GL_INT, castMapFunc<GLint, GLint>},
			{GL_UNSIGNED_INT, castMapFunc<GLint, GLuint>},
			{GL_FLOAT, castMapFunc<GLint, GLfloat>},
			{GL_DOUBLE, castMapFunc<GLint, GLdouble>}
		}},
		{GL_UNSIGNED_INT, std::unordered_map<GLenum, std::vector<char>(*)(std::vector<char>)> {
			{GL_BYTE, castMapFunc<GLuint, GLbyte>},
			{GL_UNSIGNED_BYTE, castMapFunc<GLuint, GLubyte>},
			{GL_SHORT, castMapFunc<GLuint, GLshort>},
			{GL_UNSIGNED_SHORT, castMapFunc<GLuint, GLushort>},
			{GL_INT, castMapFunc<GLuint, GLint>},
			{GL_UNSIGNED_INT, castMapFunc<GLuint, GLuint>},
			{GL_FLOAT, castMapFunc<GLuint, GLfloat>},
			{GL_DOUBLE, castMapFunc<GLuint, GLdouble>}
		}},
		{GL_FLOAT, std::unordered_map<GLenum, std::vector<char>(*)(std::vector<char>)> {
			{GL_BYTE, castMapFunc<GLfloat, GLbyte>},
			{GL_UNSIGNED_BYTE, castMapFunc<GLfloat, GLubyte>},
			{GL_SHORT, castMapFunc<GLfloat, GLshort>},
			{GL_UNSIGNED_SHORT, castMapFunc<GLfloat, GLushort>},
			{GL_INT, castMapFunc<GLfloat, GLint>},
			{GL_UNSIGNED_INT, castMapFunc<GLfloat, GLuint>},
			{GL_FLOAT, castMapFunc<GLfloat, GLfloat>},
			{GL_DOUBLE, castMapFunc<GLfloat, GLdouble>}
		}},
		{GL_DOUBLE, std::unordered_map<GLenum, std::vector<char>(*)(std::vector<char>)> {
			{GL_BYTE, castMapFunc<GLdouble, GLbyte>},
			{GL_UNSIGNED_BYTE, castMapFunc<GLdouble, GLubyte>},
			{GL_SHORT, castMapFunc<GLdouble, GLshort>},
			{GL_UNSIGNED_SHORT, castMapFunc<GLdouble, GLushort>},
			{GL_INT, castMapFunc<GLdouble, GLint>},
			{GL_UNSIGNED_INT, castMapFunc<GLdouble, GLuint>},
			{GL_FLOAT, castMapFunc<GLdouble, GLfloat>},
			{GL_DOUBLE, castMapFunc<GLdouble, GLdouble>}
		}}
	};

}
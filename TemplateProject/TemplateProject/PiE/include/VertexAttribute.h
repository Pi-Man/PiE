#pragma once

#include <vector>
#include <initializer_list>

#include "Uniform.h"
#include "Vec.h"

enum struct Attribute_Type {
	VEC1I,
	VEC2I,
	VEC3I,
	VEC4I,
	VEC1,
	VEC2,
	VEC3,
	VEC4,
	VEC1D,
	VEC2D,
	VEC3D,
	VEC4D,
	MAT4,
	MAT4D
};

template<Attribute_Type T>
struct Attribute_Type_to_Container_Type {
	using type = void;
	enum {
		length = 0
	};
};

template<>
struct Attribute_Type_to_Container_Type<Attribute_Type::VEC1I> {
	using type = GLint;
	enum {
		length = 1
	};
};
template<>
struct Attribute_Type_to_Container_Type<Attribute_Type::VEC2I> {
	using type = GLint;
	enum {
		length = 2
	};
};
template<>
struct Attribute_Type_to_Container_Type<Attribute_Type::VEC3I> {
	using type = GLint;
	enum {
		length = 3
	};
};
template<>
struct Attribute_Type_to_Container_Type<Attribute_Type::VEC4I> {
	using type = GLint;
	enum {
		length = 4
	};
};

template<>
struct Attribute_Type_to_Container_Type<Attribute_Type::VEC1> {
	using type = GLfloat;
	enum {
		length = 1
	};
};
template<>
struct Attribute_Type_to_Container_Type<Attribute_Type::VEC2> {
	using type = GLfloat;
	enum {
		length = 2
	};
};
template<>
struct Attribute_Type_to_Container_Type<Attribute_Type::VEC3> {
	using type = GLfloat;
	enum {
		length = 3
	};
};
template<>
struct Attribute_Type_to_Container_Type<Attribute_Type::VEC4> {
	using type = GLfloat;
	enum {
		length = 4
	};
};

template<>
struct Attribute_Type_to_Container_Type<Attribute_Type::VEC1D> {
	using type = GLdouble;
	enum {
		length = 1
	};
};
template<>
struct Attribute_Type_to_Container_Type<Attribute_Type::VEC2D> {
	using type = GLdouble;
	enum {
		length = 2
	};
};
template<>
struct Attribute_Type_to_Container_Type<Attribute_Type::VEC3D> {
	using type = GLdouble;
	enum {
		length = 3
	};
};
template<>
struct Attribute_Type_to_Container_Type<Attribute_Type::VEC4D> {
	using type = GLdouble;
	enum {
		length = 4
	};
};

template<>
struct Attribute_Type_to_Container_Type<Attribute_Type::MAT4> {
	using type = GLfloat;
	enum {
		length = 16
	};
};
template<>
struct Attribute_Type_to_Container_Type<Attribute_Type::MAT4D> {
	using type = GLdouble;
	enum {
		length = 16
	};
};

struct Attribute {
	union {
		void* v;
		GLfloat* f;
		GLint* i;
		GLdouble* d;
	} data;
	Attribute_Type type;
	GLsizei count;

	size_t byteLength;

	Attribute();

	template<Attribute_Type T>
	Attribute &make(int count = 1) {

		this->type = T;
		this->count = count;
		this->byteLength = sizeof(Attribute_Type_to_Container_Type<T>::type) * Attribute_Type_to_Container_Type<T>::length * count;

		data.v = malloc(this->byteLength);

		return *this;
	}

	template<Attribute_Type T>
	Attribute &make(const std::initializer_list<typename Attribute_Type_to_Container_Type<T>::type> value, int count = 1) {

		this->type = T;
		this->count = count;
		this->byteLength = sizeof(Attribute_Type_to_Container_Type<T>::type) * Attribute_Type_to_Container_Type<T>::length * count;

		data.v = malloc(this->byteLength);

		memcpy(data.v, value.begin(), this->byteLength);

		return *this;
	}

	void free();
};

enum struct Usage {
	LOCATION,
	DIRECTION,
	TEXTURE_LOCATION,
	OTHER,
};

struct VertexAttributeType {

	const Usage usage;
	const Attribute default_value;

	const size_t &byteLength;

	const Attribute_Type &type;

	VertexAttributeType(Usage usage, const Attribute &default_value) : usage(usage), default_value(default_value), type(default_value.type), byteLength(default_value.byteLength) {}

	VertexAttributeType(VertexAttributeType &&) = delete;
	VertexAttributeType(VertexAttributeType &) = delete;

	VertexAttributeType &operator=(VertexAttributeType &&) = delete;
	VertexAttributeType &operator=(VertexAttributeType &) = delete;

	~VertexAttributeType();
};

struct VertexFormat {
	std::vector<std::reference_wrapper<const VertexAttributeType>> attributes;
	size_t stride;

	VertexFormat(std::initializer_list<std::reference_wrapper<const VertexAttributeType>> attributes_list);
};

struct VertexData {
	VertexFormat format;
	std::vector<Attribute> attributes;
};

bool operator==(const VertexAttributeType & left, const VertexAttributeType & right);
bool operator!=(const VertexAttributeType & left, const VertexAttributeType & right);
bool operator==(const VertexFormat & left, const VertexFormat & right);
bool operator!=(const VertexFormat & left, const VertexFormat & right);
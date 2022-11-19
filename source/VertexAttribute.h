#pragma once
//
//#include <vector>
//#include <initializer_list>
//#include <tuple>
//#include <cassert>
//
//#include "Uniform.h"
//#include "Vec.h"

//template<ShaderTypes::Shader_Type T>
//struct Attribute_Type_to_Container_Type {
//	using type = void;
//	enum {
//		length = 0
//	};
//};
//
//template<>
//struct Attribute_Type_to_Container_Type<ShaderTypes::INT> {
//	using type = GLint;
//	enum {
//		length = 1
//	};
//};
//template<>
//struct Attribute_Type_to_Container_Type<ShaderTypes::VEC2I> {
//	using type = GLint;
//	enum {
//		length = 2
//	};
//};
//template<>
//struct Attribute_Type_to_Container_Type<ShaderTypes::VEC3I> {
//	using type = GLint;
//	enum {
//		length = 3
//	};
//};
//template<>
//struct Attribute_Type_to_Container_Type<ShaderTypes::VEC4I> {
//	using type = GLint;
//	enum {
//		length = 4
//	};
//};
//
//template<>
//struct Attribute_Type_to_Container_Type<ShaderTypes::FLOAT> {
//	using type = GLfloat;
//	enum {
//		length = 1
//	};
//};
//template<>
//struct Attribute_Type_to_Container_Type<ShaderTypes::VEC2> {
//	using type = GLfloat;
//	enum {
//		length = 2
//	};
//};
//template<>
//struct Attribute_Type_to_Container_Type<ShaderTypes::VEC3> {
//	using type = GLfloat;
//	enum {
//		length = 3
//	};
//};
//template<>
//struct Attribute_Type_to_Container_Type<ShaderTypes::VEC4> {
//	using type = GLfloat;
//	enum {
//		length = 4
//	};
//};
//
//template<>
//struct Attribute_Type_to_Container_Type<ShaderTypes::DOUBLE> {
//	using type = GLdouble;
//	enum {
//		length = 1
//	};
//};
//template<>
//struct Attribute_Type_to_Container_Type<ShaderTypes::VEC2D> {
//	using type = GLdouble;
//	enum {
//		length = 2
//	};
//};
//template<>
//struct Attribute_Type_to_Container_Type<ShaderTypes::VEC3D> {
//	using type = GLdouble;
//	enum {
//		length = 3
//	};
//};
//template<>
//struct Attribute_Type_to_Container_Type<ShaderTypes::VEC4D> {
//	using type = GLdouble;
//	enum {
//		length = 4
//	};
//};
//
//template<>
//struct Attribute_Type_to_Container_Type<ShaderTypes::MAT4> {
//	using type = GLfloat;
//	enum {
//		length = 16
//	};
//};
//template<>
//struct Attribute_Type_to_Container_Type<ShaderTypes::MAT4D> {
//	using type = GLdouble;
//	enum {
//		length = 16
//	};
//};
//
//template<ShaderTypes T, size_t COUNT = 1, typename Container_Type = typename Attribute_Type_to_Container_Type<T>::type, size_t Container_Length = Attribute_Type_to_Container_Type<T>::length, size_t BYTE_LENGTH = COUNT * Container_Length>
//struct Attribute {
//
//	Container_Type const * data;
//
//	constexpr size_t count = COUNT;
//	constexpr size_t byte_length = BYTE_LENGTH;
//
//	Attribute() {
//		data = (Container_Type*) calloc(1, BYTE_LENGTH);
//	}
//
//	Attribute(const std::initializer_list<Container_Type> value) {
//
//		assert(((char*)value.end() - (char*)value.begin()) == BYTE_LENGTH);
//
//		data = (Container_Type*) malloc(BYTE_LENGTH);
//
//		memcpy(data, value.begin(), BYTE_LENGTH);
//
//	}
//
//	~Attribute() {
//		free(data);
//	}
//};
//
//enum struct Usage {
//	LOCATION,
//	DIRECTION,
//	TEXTURE_LOCATION,
//	OTHER,
//};
//
//struct VertexAttributeBase {
//	virtual const size_t getByteLength() const = delete;
//};
//
//template<ShaderTypes T, size_t COUNT = 1>
//struct VertexAttributeType : VertexAttributeBase {
//
//	const Usage usage;
//	const Attribute<T, COUNT> default_value;
//
//	VertexAttributeType(Usage usage, const Attribute<T, COUNT> && default_value) : usage(usage), default_value(default_value) {}
//
//	VertexAttributeType(VertexAttributeType &&) = delete;
//	VertexAttributeType(VertexAttributeType &) = delete;
//
//	VertexAttributeType &operator=(VertexAttributeType &&) = delete;
//	VertexAttributeType &operator=(VertexAttributeType &) = delete;
//
//	size_t getByteLength() const override {
//		return default_value.byte_length;
//	}
//
//};
//
//struct VertexFormat {
//	std::vector<VertexAttributeBase const *> attribute_types;
//	size_t stride = 0;
//
//	VertexFormat(std::initializer_list<VertexAttributeBase const *> attribute_types) : attribute_types(attribute_types) {
//		for (const VertexAttributeBase const * attribute_type : attribute_types) {
//			stride += attribute_type->getByteLength();
//		}
//	}
//
//	template<size_t INDEX>
//	auto get() {
//		return std::get<INDEX>(default_attributes);
//	}
//};
//
//template<typename FORMAT>
//struct VertexData {
//	std::tuple<FORMAT::Types...> attributes;
//};
//
////bool operator==(const VertexAttributeType & left, const VertexAttributeType & right);
////bool operator!=(const VertexAttributeType & left, const VertexAttributeType & right);
////bool operator==(const VertexFormat & left, const VertexFormat & right);
////bool operator!=(const VertexFormat & left, const VertexFormat & right);

#include <vector>
#include <unordered_map>
#include <algorithm>

#include <glew.h>

#include "ShaderTypes.h"
#include "DefaultVertexAttributeTypes.h"

enum struct Usage {
	POSITION,
	NORMAL,
	TANGENT,
	UV,
	COLOR,
	UNSPECIFIED,
	USER_DEFINED_START,
};

struct VertexAttributeType {
	Usage usage;
	ShaderTypes::Shader_Type type;
	char default_data[sizeof(GLdouble) * 16];
	size_t size;
	size_t count;

	VertexAttributeType(Usage usage, ShaderTypes::Shader_Type type);
	template<typename T>
	VertexAttributeType(Usage usage, ShaderTypes::Shader_Type type, std::initializer_list<T> data) : VertexAttributeType(usage, type) {
		memmove(default_data, data.begin(), std::min<size_t>((char*)data.end() - (char*)data.begin(), size));
	}

	bool operator==(const VertexAttributeType & other) const;
	bool operator!=(const VertexAttributeType & other) const;
};

struct VertexFormat {
	std::vector<VertexAttributeType> types;
	std::vector<size_t> offsets;
	size_t stride;
	size_t position_index = -1;
	size_t normal_index = -1;
	size_t uv_index = -1;

	VertexFormat(std::initializer_list<VertexAttributeType> types);

	bool operator==(const VertexFormat & other) const;
	bool operator!=(const VertexFormat & other) const;
};

namespace VertexUtility {

	template<typename TO, typename FROM>
	std::vector<char> castMapFunc(std::vector<char> in) {
		size_t num_elem = in.size() / sizeof(FROM);
		FROM * from = (FROM*)in.data();
		TO * to = (TO*)malloc(sizeof(TO) * num_elem);
		if (!to) return {};
		for (size_t i = 0; i < num_elem; i++) {
			to[i] = (TO)from[i];
		}
		std::vector<char> out{ (char*)to, (char*)(to + num_elem) };
		free(to);
		return out;
	}

	extern std::unordered_map<GLenum, std::unordered_map<GLenum, std::vector<char>(*)(std::vector<char>)>> castMap;

}

struct Vertex {

	VertexFormat format;
	char * data = nullptr;
	std::vector<char> exists;

	Vertex() : Vertex({ POSITION_F }) {}

	Vertex(VertexFormat format) : Vertex(format, std::initializer_list<int>{}) {}

	template<typename ...T1>
	Vertex(VertexFormat format, std::initializer_list<T1>... rest) : Vertex(format, 0, 0, rest...) {}

	template<typename T1, typename ...T2>
	Vertex(VertexFormat format, size_t offset, size_t index, std::initializer_list<T1> first, std::initializer_list<T2>... rest) : Vertex(format, offset + format.types[index].size, index + 1, rest...) {
		if (first.begin() != first.end()) {
			memmove(data + offset, first.begin(), std::min<size_t>(sizeof(T1) * first.size(), format.types[index].size));
			exists[index] = true;
		}
	}

	template<typename T1>
	Vertex(VertexFormat format, size_t offset, size_t index, std::initializer_list<T1> first) : format(format), exists(format.types.size(), false), data((char*)malloc(format.stride)) {
		if (first.begin() != first.end()) {
			memmove(data + offset, first.begin(), std::min<size_t>(sizeof(T1) * first.size(), format.types[index].size));
			exists[index] = true;
		}
	}

	template<typename T>
	void setAttribute(size_t id, std::initializer_list<T> attribute) {
		GLenum from = ShaderTypes::gl_type_to_enum<T>::type;
		GLenum to = ShaderTypes::container_of(format.types[id].type);
		std::vector<char> d = VertexUtility::castMap[to][from]({ (char*)attribute.begin(), (char*)(attribute.begin() + attribute.size()) });
		memmove(data + format.offsets[id], &*d.begin(), std::min<size_t>(d.size(), format.types[id].size));
		exists[id] = true;
	}

	template<typename T>
	void setAttribute(size_t id, std::vector<T> attribute) {
		GLenum from = ShaderTypes::gl_type_to_enum<T>::type;
		GLenum to = ShaderTypes::container_of(format.types[id].type);
		std::vector<char> d = VertexUtility::castMap[to][from]({ (char*)attribute.data(), (char*)(attribute.data() + attribute.size()) });
		memmove(data + format.offsets[id], &*d.begin(), std::min<size_t>(d.size(), format.types[id].size));
		exists[id] = true;
	}

	template<typename T>
	std::vector<T> getAttributeAs(size_t id) const {
		GLenum from = ShaderTypes::container_of(format.types[id].type);
		GLenum to = ShaderTypes::gl_type_to_enum<T>::type;
		std::vector<char> d = VertexUtility::castMap[to][from]({ data + format.offsets[id], data + format.offsets[id + 1] });
		return std::vector<T>((T*)&*d.begin(), (T*)&*d.end());
	}

	Vertex(const Vertex & other);
	Vertex(const Vertex && other);

	~Vertex();

	Vertex & operator=(const Vertex & other);
	Vertex & operator=(const Vertex && other);
};

//struct AttributeType {
//	Usage usage;
//	ShaderTypes::Shader_Type type;
//};
//
//struct VertexFormat {
//	std::vector<AttributeType> format;
//};
//
//struct Attribute {
//	AttributeType type;
//	char data[sizeof(GLdouble) * 16];
//	size_t length;
//
//	Attribute(AttributeType type) : type(type), length(ShaderTypes::length_map[type.type]) {
//		memset(data, 0, sizeof(data));
//	}
//
//	template<typename T>
//	Attribute(AttributeType type, std::initializer_list<T> dataIn) {
//		size_t length = std::min(dataIn.end() - dataIn.begin(), ShaderTypes::length_map[type.type]);
//
//	}
//};
//
//template<ShaderTypes::Shader_Type T>
//struct VertexAttributeType {
//	using Type = Attribute_Type_to_Container_Type<T>::type;
//	using Length = Attribute_Type_to_Container_Type<T>::length;
//};
//
//template<typename T1, typename ...T>
//struct VertexFormat {
//	using Type = T1;
//	using Next = VertexFormat<T...>
//};
//template<typename T1>
//struct VertexFormat {
//	using Type = T1;
//};
//
//template<size_t IDX, typename T, typename = typename std::enable_if<IDX >= 1>::type>
//struct Vertex_Format_Get {
//	using Type = Vertex_Format_Get<IDX - 1, T::Next>::Type;
//	enum {
//		Offset = T::Type::Length * sizeof(T::Type::Type) + Vertex_Format_Get<IDX - 1, T::Next>::Offset
//	};
//};
//template<size_t IDX, typename T, typename = typename std::enable_if<IDX == 0>::type>
//struct Vertex_Format_Get {
//	using Type = T::Type;
//	enum {
//		Offset = T::Type::Length * sizeof(T::Type::Type)
//	}
//};
//
//template<typename T1, typename ...T>
//struct Vertex_Data_Length {
//	enum {
//		length = T1::Length * sizeof(T1::Type) + Vertex_Data_Length<T...>::length;
//	}
//};
//template<typename  T1>
//struct Vertex_Data_Length {
//	enum {
//		length = T1::Length * sizeof(T1::Type);
//	}
//};
//
//template<typename ...T>
//struct Vertex {
//	using Format = VertexFormat<T...>;
//	char data[Vertex_Data_Length<T...>::length];
//
//	template<typename T2, typename ...T3>
//	Vertex(std::initializer_list<T2> first, T3... rest) {
//
//	}
//
//	template<size_t IDX>
//	typename Vertex_Format_Get<IDX, Format>::Type::Type * get() {
//		return (typename Vertex_Format_Get<IDX, Format>::Type::Type *) (data + Vertex_Format_Get<IDX, Format>::Offset);
//	}
//};

/*
template<typename Vertex_T>
void addTriangle(Vertex_T v1, Vertex_T v2, Vertex_T v3)
*/
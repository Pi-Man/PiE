#include "VertexAttribute.h"

Attribute::Attribute() {
	data.v = nullptr;
}

void Attribute::free() {
	std::free(data.v);
	data.v = nullptr;
}

VertexAttributeType::~VertexAttributeType() {
	const_cast<Attribute&>(default_value).free();
}

VertexFormat::VertexFormat(std::initializer_list<std::reference_wrapper<const VertexAttributeType>> attributes_list) : attributes(attributes_list) {
	stride = 0;
	for (const VertexAttributeType & attribute : attributes) {
		stride += attribute.byteLength;
	}
}

bool operator==(const VertexAttributeType & left, const VertexAttributeType & right) {
	return &left == &right;
}

bool operator!=(const VertexAttributeType & left, const VertexAttributeType & right) {
	return !(left == right);
}

bool operator==(const VertexFormat & left, const VertexFormat & right) {

	if (left.attributes.size() != right.attributes.size() || left.stride != right.stride) return false;

	for (size_t i = 0; i < left.attributes.size(); i++) {
		if (left.attributes[i] != right.attributes[i]) return false;
	}

	return true;
}

bool operator!=(const VertexFormat & left, const VertexFormat & right) {
	return !(left == right);
}

#include <string>
#include <map>
#include <vector>

namespace JSON {

	struct Key {
		std::string key;
	};

	enum TYPE {
		ERROR = -1,
		UNSET,
		OBJECT,
		ARRAY,
		STRING,
		NUMBER,
	};

	struct Element {
		Key name; 
		TYPE type;
		union {
			std::map<std::string, Element> children;
			std::vector<Element> elements;
			std::string string;
			double number;
		};

		Element();
		Element(Key name, TYPE type);

		Element(const Element &o);
		Element(const Element &&o);
		Element &operator=(const Element &o);
		Element &operator=(const Element &&o);
		~Element();

		Element(std::initializer_list<Element> elements);
		Element(std::initializer_list<std::pair<Key, Element>> pairs);
		Element(std::initializer_list<std::pair<std::string, Element>> pairs);
		Element(const std::string &value);
		Element(const char *value);
		Element(const double value);

		Element &operator=(std::initializer_list<Element> elements);
		Element &operator=(std::initializer_list<std::pair<Key, Element>> pairs);
		Element &operator=(std::initializer_list<std::pair<std::string, Element>> pairs);
		Element &operator=(std::string &value);
		Element &operator=(const char *value);
		Element &operator=(double value);

		Element &operator[](const std::string &name);
		Element &operator[](const char *name);
		Element &operator[](const int index);

		operator double();
		operator std::string();

		static const Element Invalid;
	};


	std::initializer_list<Element> asArray(std::initializer_list<Element> elements);

	std::initializer_list<std::pair<Key, Element>> asObject(std::initializer_list<std::pair<Key, Element>> pairs);
	std::initializer_list<std::pair<std::string, Element>> asObject(std::initializer_list<std::pair<std::string, Element>> pairs);

	Element parse(const std::string &&fileName);
	Element parse(const std::string &fileName);

	void print(FILE *file, const Element &element);
	void print(const Element &element);
}

const JSON::Key operator""_key(const char* key, size_t hmm);
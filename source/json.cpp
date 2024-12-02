#include <cctype>

#include "json.h"

namespace PiE {

	const JSON::Element JSON::Element::Invalid{ "INVALID"_key, JSON::ERROR };

	JSON::Element::Element() : Element(""_key, TYPE::UNSET) {}

	JSON::Element::Element(Key name, JSON::TYPE type) : name(name), type(type) {
		switch (type) {
		case ERROR:
			number = 0;
			break;
		case UNSET:
			number = 0;
			break;
		case OBJECT:
			new (&children) std::map<std::string, Element>{};
			break;
		case ARRAY:
			new (&elements) std::vector<Element>{};
			break;
		case STRING:
			new (&string) std::string{ "" };
			break;
		case NUMBER:
			number = 0;
			break;
		default:
			number = 0;
		}
	}

	JSON::Element::Element(const Element & o) : name(o.name), type(o.type) {
		switch (type) {
		case ERROR:
			number = 0;
			break;
		case UNSET:
			number = 0;
			break;
		case OBJECT:
			new (&children) std::map<std::string, Element>{ o.children };
			break;
		case ARRAY:
			new (&elements) std::vector<Element>{ o.elements };
			break;
		case STRING:
			new (&string) std::string{ o.string };
			break;
		case NUMBER:
			number = o.number;
			break;
		default:
			number = 0;
		}
	}

	JSON::Element::Element(const Element && o) : name(std::move(o.name)), type(std::move(o.type)) {
		switch (type) {
		case ERROR:
			number = 0;
			break;
		case UNSET:
			number = 0;
			break;
		case OBJECT:
			new (&children) std::map<std::string, Element>{ std::move(o.children) };
			break;
		case ARRAY:
			new (&elements) std::vector<Element>{ std::move(o.elements) };
			break;
		case STRING:
			new (&string) std::string{ std::move(o.string) };
			break;
		case NUMBER:
			number = std::move(o.number);
			break;
		default:
			number = 0;
		}
	}

	JSON::Element & JSON::Element::operator=(const Element & o) {
		this->~Element();
		name = o.name;
		type = o.type;
		switch (type) {
		case ERROR:
			number = 0;
			break;
		case UNSET:
			number = 0;
			break;
		case OBJECT:
			new (&children) std::map<std::string, Element>{ o.children };
			break;
		case ARRAY:
			new (&elements) std::vector<Element>{ o.elements };
			break;
		case STRING:
			new (&string) std::string{ o.string };
			break;
		case NUMBER:
			number = o.number;
			break;
		default:
			number = 0;
		}
		return *this;
	}

	JSON::Element & JSON::Element::operator=(const Element && o) {
		this->~Element();
		name = std::move(o.name);
		type = std::move(o.type);
		switch (type) {
		case ERROR:
			number = 0;
			break;
		case UNSET:
			number = 0;
			break;
		case OBJECT:
			new (&children) std::map<std::string, Element>{ std::move(o.children) };
			break;
		case ARRAY:
			new (&elements) std::vector<Element>{ std::move(o.elements) };
			break;
		case STRING:
			new (&string) std::string{ std::move(o.string) };
			break;
		case NUMBER:
			number = std::move(o.number);
			break;
		default:
			number = 0;
		}
		return *this;
	}

	JSON::Element::~Element() {
		switch (type) {
		case ERROR:
			break;
		case UNSET:
			break;
		case OBJECT:
			this->children.~map();
			break;
		case ARRAY:
			this->elements.~vector();
			break;
		case STRING:
			this->string.~string();
			break;
		case NUMBER:
			break;
		default:
			break;
		}
	}

	JSON::Element::Element(std::initializer_list<Element> elementsIn) : Element(""_key, JSON::ARRAY) {
		size_t index = 0;
		for (Element element : elementsIn) {
			element.name = { std::to_string(index) };
			elements.push_back(element);
			index++;
		}
	}

	JSON::Element::Element(std::initializer_list<std::pair<Key, Element>> pairs) : Element(""_key, JSON::OBJECT) {
		for (std::pair<Key, Element> pair : pairs) {
			pair.second.name = pair.first;
			children.insert({ pair.first.key, pair.second });
		}
	}

	JSON::Element::Element(std::initializer_list<std::pair<std::string, Element>> pairs) : Element(""_key, JSON::OBJECT) {
		for (std::pair<std::string, Element> pair : pairs) {
			pair.second.name = { pair.first };
			children.insert(pair);
		}
	}

	JSON::Element::Element(const std::string & value) : Element(""_key, JSON::STRING) {
		string = value;
	}

	JSON::Element::Element(const char * value) : Element(""_key, JSON::STRING) {
		string = value;
	}

	JSON::Element::Element(double value) : Element(""_key, JSON::NUMBER) {
		number = value;
	}

	JSON::Element & JSON::Element::operator=(std::initializer_list<Element> elementsIn) {
		switch (type) {
		case JSON::ERROR:
			_ASSERT_EXPR(false, L"Cannot assign to invalid JSON::Element");
			break;
		case JSON::UNSET:
			type = JSON::ARRAY;
			{
				size_t index = 0;
				new (&elements) std::vector<JSON::Element>{};
				for (Element element : elementsIn) {
					element.name = { std::to_string(index) };
					elements.push_back(element);
					index++;
				}
			}
			break;
		case JSON::OBJECT:
			_ASSERT_EXPR(false, L"Cannot assign JSON::ARRAY to JSON::OBJECT");
			break;
		case JSON::ARRAY:
		{
			size_t index = 0;
			elements.clear();
			for (Element element : elementsIn) {
				element.name = { std::to_string(index) };
				elements.push_back(element);
				index++;
			}
		}
		break;
		case JSON::STRING:
			_ASSERT_EXPR(false, L"Cannot assign JSON::ARRAY to JSON::STRING");
			break;
		case JSON::NUMBER:
			_ASSERT_EXPR(false, L"Cannot assign JSON::ARRAY to JSON::NUMBER");
			break;
		default:
			_ASSERT_EXPR(false, L"Cannot assign to unknown type");
			break;
		}
		return *this;
	}

	JSON::Element & JSON::Element::operator=(std::initializer_list<std::pair<Key, Element>> pairs) {
		switch (type) {
		case JSON::ERROR:
			_ASSERT_EXPR(false, L"Cannot assign to invalid JSON::Element");
			break;
		case JSON::UNSET:
			type = JSON::OBJECT;
			new (&children) std::map<std::string, JSON::Element>{};
			for (std::pair<Key, Element> pair : pairs) {
				pair.second.name = pair.first;
				children.insert({ pair.first.key, pair.second });
			}
			break;
		case JSON::OBJECT:
			children.clear();
			for (std::pair<Key, Element> pair : pairs) {
				pair.second.name = pair.first;
				children.insert({ pair.first.key, pair.second });
			}
			break;
		case JSON::ARRAY:
			_ASSERT_EXPR(false, L"Cannot assign JSON::OBJECT to JSON::ARRAY");
			break;
		case JSON::STRING:
			_ASSERT_EXPR(false, L"Cannot assign JSON::OBJECT to JSON::STRING");
			break;
		case JSON::NUMBER:
			_ASSERT_EXPR(false, L"Cannot assign JSON::OBJECT to JSON::NUMBER");
			break;
		default:
			_ASSERT_EXPR(false, L"Cannot assign to unknown type");
			break;
		}
		return *this;
	}

	JSON::Element & JSON::Element::operator=(std::initializer_list<std::pair<std::string, Element>> pairs) {
		switch (type) {
		case JSON::ERROR:
			_ASSERT_EXPR(false, L"Cannot assign to invalid JSON::Element");
			break;
		case JSON::UNSET:
			type = JSON::OBJECT;
			new (&children) std::map<std::string, JSON::Element>{};
			for (std::pair<std::string, Element> pair : pairs) {
				pair.second.name = { pair.first };
				children.insert(pair);
			}
			break;
		case JSON::OBJECT:
			children.clear();
			for (std::pair<std::string, Element> pair : pairs) {
				pair.second.name = { pair.first };
				children.insert(pair);
			}
			break;
		case JSON::ARRAY:
			_ASSERT_EXPR(false, L"Cannot assign JSON::OBJECT to JSON::ARRAY");
			break;
		case JSON::STRING:
			_ASSERT_EXPR(false, L"Cannot assign JSON::OBJECT to JSON::STRING");
			break;
		case JSON::NUMBER:
			_ASSERT_EXPR(false, L"Cannot assign JSON::OBJECT to JSON::NUMBER");
			break;
		default:
			_ASSERT_EXPR(false, L"Cannot assign to unknown type");
			break;
		}
		return *this;
	}

	JSON::Element & JSON::Element::operator=(std::string & value) {
		switch (type) {
		case JSON::ERROR:
			_ASSERT_EXPR(false, L"Cannot assign to invalid JSON::Element");
			break;
		case JSON::UNSET:
			type = JSON::STRING;
			new (&string) std::string{ value };
			break;
		case JSON::OBJECT:
			_ASSERT_EXPR(false, L"Cannot assign JSON::STRING to JSON::OBJECT");
			break;
		case JSON::ARRAY:
			_ASSERT_EXPR(false, L"Cannot assign JSON::STRING to JSON::ARRAY");
			break;
		case JSON::STRING:
			string = value;
			break;
		case JSON::NUMBER:
			number = std::stod(value);
			break;
		default:
			_ASSERT_EXPR(false, L"Cannot assign to unknown type");
			break;
		}
		return *this;
	}

	JSON::Element & JSON::Element::operator=(const char * value) {
		switch (type) {
		case JSON::ERROR:
			_ASSERT_EXPR(false, L"Cannot assign to invalid JSON::Element");
			break;
		case JSON::UNSET:
			type = JSON::STRING;
			new (&string) std::string(value);
			break;
		case JSON::OBJECT:
			_ASSERT_EXPR(false, L"Cannot assign JSON::STRING to JSON::OBJECT");
			break;
		case JSON::ARRAY:
			_ASSERT_EXPR(false, L"Cannot assign JSON::STRING to JSON::ARRAY");
			break;
		case JSON::STRING:
			string = value;
			break;
		case JSON::NUMBER:
			number = std::stod(value);
			break;
		default:
			_ASSERT_EXPR(false, L"Cannot assign to unknown type");
			break;
		}
		return *this;
	}

	JSON::Element & JSON::Element::operator=(const double value) {
		switch (type) {
		case JSON::ERROR:
			_ASSERT_EXPR(false, L"Cannot assign to invalid JSON::Element");
			break;
		case JSON::UNSET:
			type = JSON::NUMBER;
			number = value;
			break;
		case JSON::OBJECT:
			_ASSERT_EXPR(false, L"Cannot assign JSON::NUMBER to JSON::OBJECT");
			break;
		case JSON::ARRAY:
			_ASSERT_EXPR(false, L"Cannot assign JSON::NUMBER to JSON::ARRAY");
			break;
		case JSON::STRING:
			string = std::to_string(value);
			break;
		case JSON::NUMBER:
			number = value;
			break;
		default:
			_ASSERT_EXPR(false, L"Cannot assign to unknown type");
			break;
		}
		return *this;
	}

	JSON::Element & JSON::Element::operator[](const std::string & name) {
		switch (type) {
		case JSON::ERROR:
			_ASSERT_EXPR(false, L"Cannot access member from invalid JSON::Element");
			break;
		case JSON::UNSET:
			_ASSERT_EXPR(false, L"Cannot get value of unset JSON::Element");
			break;
		case JSON::OBJECT:
			return children[name];
			break;
		case JSON::ARRAY:
			return elements[std::stoull(name)];
			break;
		case JSON::STRING:
			_ASSERT_EXPR(false, L"Cannot access member of a JSON::STRING");
			break;
		case JSON::NUMBER:
			_ASSERT_EXPR(false, L"Cannot access member of a JSON::NUMBER");
			break;
		default:
			_ASSERT_EXPR(false, L"Cannot get value of unknown type");
			break;
		}
		return *this;
	}

	JSON::Element & JSON::Element::operator[](const char * name) {
		switch (type) {
		case JSON::ERROR:
			_ASSERT_EXPR(false, L"Cannot access member from invalid JSON::Element");
			break;
		case JSON::UNSET:
			_ASSERT_EXPR(false, L"Cannot get value of unset JSON::Element");
			break;
		case JSON::OBJECT:
			return children[name];
			break;
		case JSON::ARRAY:
			return elements[std::stoull(name)];
			break;
		case JSON::STRING:
			_ASSERT_EXPR(false, L"Cannot access member of a JSON::STRING");
			break;
		case JSON::NUMBER:
			_ASSERT_EXPR(false, L"Cannot access member of a JSON::NUMBER");
			break;
		default:
			_ASSERT_EXPR(false, L"Cannot get value of unknown type");
			break;
		}
		return *this;
	}

	JSON::Element & JSON::Element::operator[](const int index) {
		switch (type) {
		case JSON::ERROR:
			_ASSERT_EXPR(false, L"Cannot access member from invalid JSON::Element");
			break;
		case JSON::UNSET:
			_ASSERT_EXPR(false, L"Cannot index an unset JSON::Element");
			break;
		case JSON::OBJECT:
			_ASSERT_EXPR(false, L"Cannot index a JSON::OBJECT");
			break;
		case JSON::ARRAY:
			return elements[index];
			break;
		case JSON::STRING:
			_ASSERT_EXPR(false, L"Cannot index a JSON::STRING");
			break;
		case JSON::NUMBER:
			_ASSERT_EXPR(false, L"Cannot index a JSON::NUMBER");
			break;
		default:
			_ASSERT_EXPR(false, L"Cannot get value of unknown type");
			break;
		}
		return *this;
	}

	JSON::Element::operator double() {
		switch (type) {
		case JSON::ERROR:
			_ASSERT_EXPR(false, L"Cannot retrieve value from invalid JSON::Element");
			break;
		case JSON::UNSET:
			_ASSERT_EXPR(false, L"Cannot retrieve value from an unset JSON::Element");
			break;
		case JSON::OBJECT:
			_ASSERT_EXPR(false, L"Cannot convert a JSON::OBJECT to JSON::NUMBER");
			break;
		case JSON::ARRAY:
			_ASSERT_EXPR(false, L"Cannot convert a JSON::ARRAY to JSON::NUMBER");
			break;
		case JSON::STRING:
			return std::stod(string);
			break;
		case JSON::NUMBER:
			return number;
			break;
		default:
			_ASSERT_EXPR(false, L"Cannot retrieve value from unknown type");
			break;
		}
		return 0;
	}

	JSON::Element::operator std::string() {
		switch (type) {
		case JSON::ERROR:
			_ASSERT_EXPR(false, L"Cannot retrieve value from invalid JSON::Element");
			break;
		case JSON::UNSET:
			_ASSERT_EXPR(false, L"Cannot retrieve value from an unset JSON::Element");
			break;
		case JSON::OBJECT:
			_ASSERT_EXPR(false, L"Cannot convert a JSON::OBJECT to JSON::STRING");
			break;
		case JSON::ARRAY:
			_ASSERT_EXPR(false, L"Cannot convert a JSON::ARRAY to JSON::STRING");
			break;
		case JSON::STRING:
			return string;
			break;
		case JSON::NUMBER:
			return std::to_string(number);
			break;
		default:
			_ASSERT_EXPR(false, L"Cannot retrieve value from unknown type");
			break;
		}
		return "";
	}

	inline JSON::Element parseElement(FILE * file, const std::string & nameIn);
	inline JSON::Element parseObject(FILE * file, const std::string & nameIn);
	inline JSON::Element parseArray(FILE * file, const std::string & nameIn);
	inline JSON::Element parseString(FILE * file, const std::string & nameIn);
	inline JSON::Element parseNumber(FILE * file, const std::string & nameIn);

	std::initializer_list<JSON::Element> JSON::asArray(std::initializer_list<Element> elementsIn) {
		size_t index = 0;
		for (Element element : elementsIn) {
			element.name = { std::to_string(index) };
			index++;
		}
		return elementsIn;
	}

	std::initializer_list<std::pair<JSON::Key, JSON::Element>> JSON::asObject(std::initializer_list<std::pair<Key, Element>> pairs) {
		for (std::pair<Key, Element> pair : pairs) {
			pair.second.name = pair.first;
		}
		return pairs;
	}

	std::initializer_list<std::pair<std::string, JSON::Element>> JSON::asObject(std::initializer_list < std::pair<std::string, Element >> pairs) {
		for (std::pair<std::string, Element> pair : pairs) {
			pair.second.name = { pair.first };
		}
		return pairs;
	}

	JSON::Element JSON::parse(const std::string && fileName) {
		return JSON::parse(fileName);
	}

	JSON::Element JSON::parse(const std::string & fileName) {

		FILE * file = NULL;
		errno_t e;
		if (e = fopen_s(&file, fileName.c_str(), "r")) {
			char es[255];
			strerror_s(es, e);
			printf("%s\n", es);
			return JSON::Element::Invalid; // error
		}
		char c;
		while ((c = getc(file)) != EOF) {
			switch (c) {
			case '{':
				return parseObject(file, "");
				break;
			default:
				if (std::isspace(c)) continue;
				return JSON::Element::Invalid; // error
			}
		}

		return JSON::Element::Invalid; // error
	}

	inline JSON::Element parseElement(FILE * file, const std::string & nameIn) {
		fseek(file, -1, SEEK_CUR);
		char c;
		while ((c = getc(file)) != EOF) {
			switch (c) {
			case '{':
				return parseObject(file, nameIn);
			case '[':
				return parseArray(file, nameIn);
			case '"':
				return parseString(file, nameIn);
			default:
				if (std::isspace(c)) continue;
				return parseNumber(file, nameIn);
			}
		}

		return JSON::Element::Invalid; // error
	}

	inline JSON::Element parseObject(FILE * file, const std::string & nameIn) {
		bool nameStart = false;
		bool nameEnd = false;
		bool valueStart = false;
		bool valueEnd = false;
		std::string name = "";
		JSON::Element object{ JSON::Key{ nameIn }, JSON::OBJECT };
		char c;
		while ((c = getc(file)) != EOF) {
			switch (c) {
			case '"':
				if (nameStart) {
					if (nameEnd) {
						if (valueStart) {
							if (valueEnd) {
								return JSON::Element::Invalid; // error
							}
							else {
								object.children.insert({ { name }, parseString(file, name) });
								valueEnd = true;
							}
						}
						else {
							return JSON::Element::Invalid; // error
						}
					}
					else {
						nameEnd = true;
					}
				}
				else {
					nameStart = true;
				}
				break;
			case ':':
				if (nameStart) {
					if (nameEnd) {
						if (valueStart) {
							return JSON::Element::Invalid; // error
						}
						else {
							valueStart = true;
						}
					}
					else {
						return JSON::Element::Invalid; // error
					}
				}
				else {
					return JSON::Element::Invalid; // error
				}
				break;
			case '{':
				if (valueStart) {
					if (valueEnd) {
						return JSON::Element::Invalid; // error
					}
					else {
						object.children.insert({ { name }, parseObject(file, name) });
						valueEnd = true;
					}
				}
				else {
					return JSON::Element::Invalid; // error
				}
				break;
			case '}':
				if (valueEnd) {
					return object;
				}
				else {
					return JSON::Element::Invalid; // error
				}
				break;
			case ',':
				if (valueEnd) {
					name = "";
					nameStart = false;
					nameEnd = false;
					valueStart = false;
					valueEnd = false;
				}
				break;
			case '[':
				if (valueStart) {
					if (valueEnd) {
						return JSON::Element::Invalid; // error
					}
					else {
						object.children.insert({ { name }, parseArray(file, name) });
						valueEnd = true;
					}
				}
				else {
					return JSON::Element::Invalid; // error
				}
				break;
			default:
				if (nameStart) {
					if (nameEnd) {
						if (valueStart) {
							if (valueEnd) {
								if (std::isspace(c)) continue;
								return JSON::Element::Invalid; // error
							}
							else {
								if (std::isspace(c)) continue;
								object.children.insert({ { name }, parseNumber(file, name) });
								valueEnd = true;
							}
						}
						else {
							if (std::isspace(c)) continue;
							return JSON::Element::Invalid; // error
						}
					}
					else {
						name += c;
					}
				}
				else {
					if (std::isspace(c)) continue;
					return JSON::Element::Invalid; // error
				}
			}
		}

		return JSON::Element::Invalid; // error
	}

	inline JSON::Element parseArray(FILE * file, const std::string & nameIn) {

		size_t index = 0;
		bool elementEnd = false;

		JSON::Element array{ JSON::Key{ nameIn }, JSON::ARRAY };

		char c;
		while ((c = getc(file)) != EOF) {
			switch (c) {
			case ',':
				if (elementEnd) {
					index++;
					elementEnd = false;
				}
				else {
					return JSON::Element::Invalid; // error
				}
				break;
			case ']':
				if (elementEnd) {
					return array;
				}
				else {
					return JSON::Element::Invalid; // error
				}
				break;
			default:
				if (elementEnd) {
					if (isspace(c)) continue;
					return JSON::Element::Invalid; // error
				}
				array.elements.push_back(parseElement(file, std::to_string(index)));
				elementEnd = true;
			}
		}

		return JSON::Element::Invalid; // error
	}
	inline JSON::Element parseString(FILE * file, const std::string & nameIn) {

		JSON::Element string{ JSON::Key{ nameIn }, JSON::STRING };

		char c;
		while ((c = getc(file)) != EOF) {
			switch (c) {
			case '\\':
			{
				char c2 = getc(file);
				switch (c2) {
				case 'n':
					string.string += '\n';
					break;
				case 't':
					string.string += '\t';
					break;
				case '"':
					string.string += '"';
					break;
				case '\\':
					string.string += '\\';
					break;
				default:
					string.string += c;
					string.string += c2;
				}
				break;
			}
			case '"':
				return string;
			default:
				string.string += c;
			}
		}

		return JSON::Element::Invalid; // error
	}

	inline JSON::Element parseNumber(FILE * file, const std::string & nameIn) {

		fseek(file, -1, SEEK_CUR);

		bool decimalPont = false;
		bool start = false;

		std::string numberString = "";

		JSON::Element number{ JSON::Key{ nameIn }, JSON::NUMBER };

		char c;
		while ((c = getc(file)) != EOF) {
			switch (c) {
			case '.':
				if (decimalPont) {
					fseek(file, -1, SEEK_CUR);
					number.number = std::stod(numberString);
					return number;
				}
				else {
					numberString += c;
					decimalPont = true;
					start = true;
				}
				break;
			case '_':
				break;
			default:
				if (!start && isspace(c)) continue;
				if (isdigit(c)) {
					numberString += c;
					start = true;
				}
				else {
					fseek(file, -1, SEEK_CUR);
					number.number = std::stod(numberString);
					return number;
				}
			}
		}

		return JSON::Element::Invalid; // error
	}

	void print(FILE * file, const JSON::Element & element, int level) {
		std::string s(level, '\t');
		const char * tabs = s.c_str();
		switch (element.type) {
		case JSON::OBJECT:
			fputs("{", file);
			if (element.children.size() > 0) {
				fputc('\n', file);
				for (auto it = element.children.cbegin(); it != --element.children.cend(); it++) {
					fputs(tabs, file);
					fputc('\t', file);
					fputs((*it).first.c_str(), file);
					fputs(": ", file);
					print(file, (*it).second, level + 1);
					fputs(",\n", file);
				}
				fputs(tabs, file);
				fputc('\t', file);
				fputs((*--element.children.cend()).first.c_str(), file);
				fputs(": ", file);
				print(file, (*--element.children.cend()).second, level + 1);
				fputc('\n', file);
				fputs(tabs, file);
			}
			fputs("}", file);
			break;
		case JSON::ARRAY:
			fputs("[", file);
			if (element.children.size() > 0) {
				fputc('\n', file);
				for (int i = 0; i + 1 < element.elements.size(); i++) {
					fputs(tabs, file);
					fputc('\t', file);
					print(file, element.elements[i], level + 1);
					fputs(",\n", file);
				}
				fputs(tabs, file);
				fputc('\t', file);
				print(file, element.elements[element.elements.size()-1], level + 1);
				fputc('\n', file);
				fputs(tabs, file);
			}
			fputs("]", file);
			break;
		case JSON::STRING:
			fputc('"', file);
			fputs(element.string.c_str(), file);
			fputc('"', file);
			break;
		case JSON::NUMBER:
			fputs(std::to_string(element.number).c_str(), file);
			break;
		default:
			fputs("INVALID_ELEMENT", file);
			break;
		}
	}

	void JSON::print(FILE * file, const JSON::Element & element) {
		PiE::print(file, element, 0);
		fputc('\n', file);
	}

	void JSON::print(const JSON::Element & element) {
		print(stdout, element);
	}

	const JSON::Key operator""_key(const char * key, size_t hmm) {
		return { key };
	}

}
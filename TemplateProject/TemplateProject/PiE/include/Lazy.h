#ifndef _LAZY_H_
#define _LAZY_H_

#include <functional>

template<typename T>
struct Lazy {
	T value;
	bool resolved;
	std::function<T()> resolver;

	Lazy(std::function<T()> resolver) : resolver(resolver), resolved(false), value() {}
	Lazy(std::function<T()> resolver, T defaultValue) : resolver(resolver), resolved(true), value(defaultValue) {}

	Lazy &operator=(const T &valueIn);

	void invalidate();

	operator T();

	operator bool();

	T operator()();
};

template<typename T>
Lazy<T> & Lazy<T>::operator=(const T & valueIn) {
	value = valueIn;
	resolved = true;
	return *this;
}

template<typename T>
void Lazy<T>::invalidate() {
	resolved = false;
}

template<typename T>
Lazy<T>::operator T() {
	return (*this)();
}

template<typename T>
Lazy<T>::operator bool() {
	return resolved;
}

template<typename T>
T Lazy<T>::operator()() {
	if (!resolved) {
		value = resolver();
		resolved = true;
	}
	return value;
}

#endif
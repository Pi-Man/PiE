#pragma once
#include <functional>

template<typename T>
struct Optional {
	union {
		std::max_align_t dummy;
		unsigned char c[sizeof(T)]; // allow stack allocating T without initializing
		T t;
	};

	bool has_val;

	Optional();
	Optional(T &&);
	Optional(T &);

	static const Optional<T> empty;

	T & value_or(T &);
	T value_or(T &&) const;

	template<typename R, typename F, typename _V = std::enable_if_t<std::is_same_v<std::remove_reference_t<std::remove_cv_t<std::result_of_t<F&(T&)>>>, Optional<R>>>>
	auto transform(F & f) -> decltype(f(**this)) {
		return has_val ? f(**this) : Optional<R>::empty;
	}
	template<typename R, typename F, typename _V = std::enable_if_t<std::is_same_v<std::remove_reference_t<std::remove_cv_t<std::result_of_t<F&&(T&)>>>, Optional<R>>>>
	auto transform(F && f) -> decltype(f(**this)) {
		return has_val ? f(**this) : Optional<R>::empty;
	}

	template<typename R, typename F, typename _V = std::enable_if_t<std::is_same_v<std::remove_reference_t<std::remove_cv_t<std::result_of_t<F&()>>>, Optional<R>>>>
	auto or_else(F & f) -> decltype(f()) {
		return has_val ? *this : f();
	}
	template<typename R, typename F, typename _V = std::enable_if_t<std::is_same_v<std::remove_reference_t<std::remove_cv_t<std::result_of_t<F&&()>>>, Optional<R>>>>
	auto or_else(F && f) -> decltype(f()) {
		return has_val ? *this : f();
	}

	void reset();

	operator bool() const;
	T & operator*();
	T * operator->();

	Optional<T> & operator=(const T & value);
	Optional<T> & operator=(const T && value);
};

template<typename T>
const Optional<T> Optional<T>::empty{};

template<typename T>
inline Optional<T>::Optional() : has_val(false) {}

template<typename T>
inline Optional<T>::Optional(T && t) : has_val(true), t(std::move(t)) {}

template<typename T>
inline Optional<T>::Optional(T & t) : has_val(true), t(t) {}

template<typename T>
inline T & Optional<T>::value_or(T & default_value) {
	return has_val ? t : default_value;
}

template<typename T>
inline T Optional<T>::value_or(T && default_value) const {
	return has_val ? t : default_value;
}

template<typename T>
inline void Optional<T>::reset() {
	has_val = false;
}

template<typename T>
inline Optional<T>::operator bool() const {
	return has_val;
}

template<typename T>
inline T & Optional<T>::operator*() {
	return t;
}

template<typename T>
inline T * Optional<T>::operator->() {
	return &t;
}

template<typename T>
inline Optional<T>& Optional<T>::operator=(const T & value) {
	t = value;
	has_val = true;
	return *this;
}

template<typename T>
inline Optional<T>& Optional<T>::operator=(const T && value) {
	t = value;
	has_val = true;
	return *this;
}
#ifndef _ARRAY_H
#define _ARRAY_H

#include <cstddef>

template <typename T, std::size_t N>
class Array {
public:
	Array() {}
	Array(const T orig[N]) {
		for (std::size_t i = 0; i < N; i++)
			elems[i] = orig[i];
	}

	T elems[N];

	T& operator[](std::size_t n) {
		return elems[n];
	}
	const T& operator[](std::size_t n) const {
		return elems[n];
	}
	static std::size_t size() { return N; }
};

#endif

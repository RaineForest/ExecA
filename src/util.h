
#ifndef _UTIL_H_
#define _UTIL_H_

#include <climits>
#include <cstdint>

using namespace std;

typedef enum {
	BIG,
	LITTLE
} Endianness;

Endianness getSystemEndianness();

template <typename T>
T change_endian(T num) {
	union {
		T n;
		uint8_t n8[sizeof(T)];
	} src, dst;

	src.n = num;
	for(int i = 0; i < sizeof(T); i++) {
		dst.n8[i] = src.n8[sizeof(T)-i-1];
	}

	return dst.n;
}

template <typename T>
T correct_endian(T num, Endianness numEnd) {
	if(getSystemEndianness() == numEnd) {
		return num;
	} else {
		return change_endian<T>(num);
	}
}

#endif



#ifndef _UTIL_H_
#define _UTIL_H_

#include <climits>

using namespace std;

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

#endif


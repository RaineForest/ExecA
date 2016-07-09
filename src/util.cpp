
#include "util.h"

Endianness getSystemEndianness() {
	int n = 1;
	if(*(char*)&n == 1) {
		return LITTLE;
	} else {
		return BIG;
	}
}

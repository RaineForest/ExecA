
#include "elf.h"
#include <cstdio>

int main() {
	ELF* e = new ELF();

	try {
		e->read("filename");
	} catch(string* str) {
		printf("Error: %s\n", str->c_str());
		delete e;
		return 1;
	}

	delete e;

	return 0;
}

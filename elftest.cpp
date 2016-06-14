
#include "elf.h"
#include <cstdio>

int main(int argc, char** argv) {
	if(argc != 2) {
		printf("Usage: elftest <elf file>\n");
		return 1;
	}

	ELF* e = new ELF();

	try {
		e->read(argv[1]);
	} catch(string* str) {
		printf("Error: %s\n", str->c_str());
		delete e;
		return 1;
	}

	delete e;

	return 0;
}

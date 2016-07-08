
#include "elf.h"
#include <cstdio>

int main(int argc, char** argv) {
	if(argc != 2) {
		printf("Usage: elftest <elf file>\n");
		return 1;
	}

	ELF32* e = new ELF32();

	try {
		e->read(argv[1]);
	} catch(string* str) {
		printf("Error: %s\n", str->c_str());
		delete e;
		return 1;
	}

	e->printHeaderInfo();

	printf("\nType\tOffset\tVirtAdr\t\tPhysAdr\t\tFileSiz\t\tMemSiz\t\tFlg\tAlign\n");
	for(int i = 0; i < e->getNumProgramHeaders(); i++) {
		e->printProgramHeaderInfo(i);
	}

	printf("\nNameOff\tType\tAddr\t\tOff\tSize\tES\tFlg\t\tLk\tInf\tAl\n");
	for(int i = 0; i < e->getNumSections(); i++) {
		e->printSectionHeaderInfo(i);
	}

	delete e;

	return 0;
}

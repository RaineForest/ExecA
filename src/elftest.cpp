
#include "elf32.h"
#include <cstdio>
#include <cstdlib>
#include <cstdint>

int main(int argc, char** argv) {
	if(argc != 4) {
		printf("Usage: elftest <elf file> <section #> <dump file>\n");
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

	int secNum = atoi(argv[2]);
	if(secNum < 0 || secNum >= e->getNumSections()) {
		printf("not a valid section number");
		delete e;
		return 1;
	}
	int size = e->getSectionSize(secNum);
	uint8_t* buffer = new uint8_t[size];
	e->getSection(secNum, buffer, size);

	FILE* fid = fopen(argv[3], "wb");
	fwrite(buffer, sizeof(uint8_t), size, fid);
	fclose(fid);

	delete e;

	return 0;
}

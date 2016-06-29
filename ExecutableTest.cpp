
//compile with: g++ ExecutableManager.cpp ExecutableTest.cpp -o ExecutableTest -lbfd

#include <cstdio>
#include <string>
#include "ExecutableManager.h"

int main(int argc, char** argv) {
	if(argc != 2) {
		printf("Usage: ExecutableTest <file>\n");
		return 1;
	}

	ExecutableManager* em = new ExecutableManager();

	if(em->read(string(argv[1]))) {
		delete em;
		return 1;
	}

	printf("1\n");
	const int SECTION = 1;
	int secSize = em->getSectionSize(SECTION);
	if(secSize == -1) {
		delete em;
		return 1;
	}
	printf("2\n");
	uint8_t* sec = new uint8_t[secSize];
	if(em->getSection(SECTION, sec, secSize)) {
		delete em;
		return 1;
	}
	printf("3\n");

	delete em;

	return 0;
}

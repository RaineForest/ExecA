
#include <cstdio>
#include <string>
#include "ExecutableManager.h"

int main(int argc, char** argv) {
	if(argc != 2) {
		printf("Usage: ExecutableTest <file>\n");
		return 1;
	}

	ExecutableManager* em = new ExecutableManager();

	em->read(string(argv[1]));

	delete em;

	return 0;
}

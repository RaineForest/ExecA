
#ifndef _EXECUTABLEMANAGER_H_
#define _EXECUTABLEMANAGER_H_

#define PACKAGE 1
#define PACKAGE_NAME 1

#include <cstdint>
#include <string>

#include <bfd.h>

using namespace std;

class ExecutableManager {

public:
	ExecutableManager();
	~ExecutableManager();

	int read(string filename);

	int getSectionSize(int n);
	int getSection(int n, void* contents, int contentsLen);

private:
	bfd* bfdHandle;
};

#endif


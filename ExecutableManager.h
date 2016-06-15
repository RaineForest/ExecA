
#ifndef _EXECUTABLEMANAGER_H_
#define _EXECUTABLEMANAGER_H_

#include <cstdint>
#include <string>

#include <bfd.h>

using namespace std;

class ExecutableManager {

public:
	ExecutableManager();
	~ExecutableManager();

	int read(string filename);
	void getSectionNames();
	void getSectionTypes();
	void getSection(int n);

private:
	bfd* bfdHandle;
};

#endif


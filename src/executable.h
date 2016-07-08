
#ifndef _EXECUTABLE_H_
#define _EXECUTABLE_H_

#include <string>

using namespace std;

typedef enum {
	BIG,
	LITTLE
} Endianness;

class Executable {

public:
	virtual void read(string filename) = 0;

	virtual void getSection(int sectionNum, uint8_t* buffer, unsigned int len) const = 0;
	virtual int getNumSections() const = 0;

	virtual Endianness getEndianness() const = 0;

	//virtual InstructionSet* getArch() = 0;
};

#endif


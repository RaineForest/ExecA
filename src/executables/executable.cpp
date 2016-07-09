
#include "executable.h"
#include "elf32.h"
#include "elf64.h"

Executable* makeExecutable(string filename) {
	if(ELF32::isOfType(filename)) {
		ELF32* file = new ELF32;
		file->read(filename);
		return file;
	}

	if(ELF64::isOfType(filename)) {
		ELF64* file = new ELF64;
		file->read(filename);
		return file;
	}

	return nullptr;
}


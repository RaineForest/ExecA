
#include "elf.h"
#include <cstdio>
#include <string>

using namespace std;

ELF::ELF() {
	header = new ELF_Header;
	entry = nullptr;
	ph_off = nullptr;
	sh_off = nullptr;
}

ELF::~ELF() {
	delete header;
	delete entry;
	delete ph_off;
	delete sh_off;
}

int ELF::getWordSize() {
	return header->bits == 1 ? 4 : 8;
}

void ELF::read(string filename) {
	FILE* file = fopen(filename.c_str(), "rb");
	if(!file) {
		throw new string("Could not read file");
	}
		
	fread(header, sizeof(*header), 1, file);

	delete entry;
	delete ph_off;
	delete sh_off;
	if(header->bits == 1) {
		entry = new uint8_t[4];
		ph_off = new uint8_t[4];
		sh_off = new uint8_t[4];
	} else {
		entry = new uint8_t[8];
		ph_off = new uint8_t[8];
		sh_off = new uint8_t[8];
	}

	fread(entry, getWordSize(), 1, file);
	fread(ph_off, getWordSize(), 1, file);
	fread(sh_off, getWordSize(), 1, file);

	fread(&flags, sizeof(flags), 1, file);
	fread(&header_size, sizeof(header_size), 1, file);
	fread(&ph_ent_size, sizeof(ph_ent_size), 1, file);
	fread(&ph_num, sizeof(ph_num), 1, file);
	fread(&sh_ent_size, sizeof(sh_ent_size), 1, file);
	fread(&sh_num, sizeof(sh_num), 1, file);
	fread(&sh_str_index, sizeof(sh_str_index), 1, file);

	fclose(file);
}

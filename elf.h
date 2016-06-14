
#ifndef _ELF_H_
#define _ELF_H_

#include "executable.h"
#include <stdint.h>
#include <string>

using namespace std;

typedef struct ELF_Header {
	uint8_t magicNum[4];
	uint8_t bits; //32 vs 64
	uint8_t endianness; // 1 = little, 2 = big
	uint8_t	version;
	uint8_t abi;
	uint8_t abi_ver;
	uint8_t padding[7];
	uint16_t type;
	uint16_t isa;
	uint32_t version2;
} ELF_Header_t;

class ELF : Executable {
public:
	ELF();
	~ELF();

	void read(string filename);

private:
	int getWordSize();

	ELF_Header_t* header;
	uint8_t* entry;
	uint8_t* ph_off; //program header offset
	uint8_t* sh_off; //section header offset
	uint32_t flags;
	uint16_t header_size;
	uint16_t ph_ent_size;
	uint16_t ph_num;
	uint16_t sh_ent_size;
	uint16_t sh_num;
	uint16_t sh_str_index;
};

#endif


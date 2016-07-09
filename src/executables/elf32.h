
#ifndef _ELF32_H_
#define _ELF32_H_

#include "executable.h"
#include <stdint.h>
#include <string>

using namespace std;

typedef struct {
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
	uint32_t entry;
	uint32_t ph_off; //program header offset
	uint32_t sh_off; //section header offset
	uint32_t flags;
	uint16_t header_size;
	uint16_t ph_ent_size;
	uint16_t ph_num;
	uint16_t sh_ent_size;
	uint16_t sh_num;
	uint16_t sh_str_index;
} ELF32_Header;

typedef struct {
	uint32_t p_type;
	uint32_t p_offset;
	uint32_t p_vaddr;
	uint32_t p_paddr;
	uint32_t p_filesz;
	uint32_t p_memsz;
	uint32_t p_flags;
	uint32_t p_align;
} ELF32_Program_Header;

typedef struct {
	uint32_t s_name;
	uint32_t s_type;
	uint32_t s_flags;
	uint32_t s_addr;
	uint32_t s_offset;
	uint32_t s_size;
	uint32_t s_link;
	uint32_t s_info;
	uint32_t s_addralign;
	uint32_t s_entsize;
} ELF32_Section_Header;

class ELF32 : public Executable {
public:
	ELF32();
	~ELF32();

	static bool isOfType(string filename);
	void read(string filename);

	int getNumProgramHeaders() const;
	int getNumSections() const;

	void getSection(int sectionNum, uint8_t* buffer, unsigned int len) const;
	int getSectionSize(int s);

	int getWordSize() const;

	void printHeaderInfo();
	void printProgramHeaderInfo(int p);
	void printSectionHeaderInfo(int s);

	Endianness getEndianness() const;

private:

	ELF32_Header* header;
	ELF32_Program_Header* pHeaders;
	ELF32_Section_Header* sHeaders;

	uint8_t* data;
	int dataLen;
};

#endif


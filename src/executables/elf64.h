
#ifndef _ELF64_H_
#define _ELF64_H_

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
	uint64_t entry;
	uint64_t ph_off; //program header offset
	uint64_t sh_off; //section header offset
	uint32_t flags;
	uint16_t header_size;
	uint16_t ph_ent_size;
	uint16_t ph_num;
	uint16_t sh_ent_size;
	uint16_t sh_num;
	uint16_t sh_str_index;
} ELF64_Header;

typedef struct {
	uint32_t p_type;
	uint32_t p_flags;
	uint64_t p_offset;
	uint64_t p_vaddr;
	uint64_t p_paddr;
	uint64_t p_filesz;
	uint64_t p_memsz;
	uint64_t p_align;
} ELF64_Program_Header;

typedef struct {
	uint32_t s_name;
	uint32_t s_type;
	uint64_t s_flags;
	uint64_t s_addr;
	uint64_t s_offset;
	uint64_t s_size;
	uint32_t s_link;
	uint32_t s_info;
	uint64_t s_addralign;
	uint64_t s_entsize;
} ELF64_Section_Header;

class ELF64 : public Executable {
public:
	ELF64();
	~ELF64();

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

	ELF64_Header* header;
	ELF64_Program_Header* pHeaders;
	ELF64_Section_Header* sHeaders;

	uint8_t* data;
	int dataLen;
};

#endif


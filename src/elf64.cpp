
#include "elf64.h"
#include "util.h"
#include <cstdio>
#include <cstring>
#include <algorithm>

using namespace std;

ELF64::ELF64() {}

ELF64::~ELF64() {
	delete[] data;
}

int ELF64::getWordSize() const {
	return header->bits == 1 ? 4 : 8;
}

bool ELF64::isOfType(string filename) {
	FILE* file = fopen(filename.c_str(), "rb");
	if(!file) {
		throw new string("Could not read file");
	}
	
	uint8_t magic[5];
	fread(magic, sizeof(uint8_t), 5, file);
	return 	magic[0] == 0x7F &&
		magic[1] == 0x45 && //'E'
		magic[2] == 0x4c && //'L'
		magic[3] == 0x46 && //'F'
		magic[4] == 0x02;   //64 bit
}

void ELF64::read(string filename) {
	FILE* file = fopen(filename.c_str(), "rb");
	if(!file) {
		throw new string("Could not read file");
	}

	fseek(file, 0, SEEK_END);
	dataLen = ftell(file);
	fseek(file, 0, SEEK_SET);

	data = new uint8_t[dataLen];

	int bytesread = fread(data, sizeof(uint8_t), dataLen, file);
	if(bytesread != dataLen) {
		printf("bytesread: %d\n", bytesread);
		throw new string("Could not read whole file");
	}

	fclose(file);
	
	header = (ELF64_Header*) &data[0];

	pHeaders = (ELF64_Program_Header*) &data[change_endian<uint64_t>(header->ph_off)];

	sHeaders = (ELF64_Section_Header*) &data[change_endian<uint64_t>(header->sh_off)];
}

int ELF64::getNumProgramHeaders() const {
	return change_endian<uint16_t>(header->ph_num);
}

void ELF64::getSection(int sectionNum, uint8_t* buffer, unsigned int len) const {
	memcpy(buffer, &data[change_endian<uint64_t>(sHeaders[sectionNum].s_offset)], 
		min((uint64_t)len, change_endian<uint64_t>(sHeaders[sectionNum].s_size)));
}

int ELF64::getSectionSize(int s) {
	return change_endian<uint64_t>(sHeaders[s].s_size);
}

int ELF64::getNumSections() const {
	return change_endian<uint16_t>(header->sh_num);
}

void ELF64::printHeaderInfo() {
	printf("ELF Header:\n \
		Magic: %.2X %.2X %.2X %.2X\n \
		Class: %s\n \
		Data: %.2X\n \
		Version: %.2X\n \
		OS/ABI: %.2X\n \
		ABI Version: %.2X\n \
		Type: %.4X\n \
		Machine: %.4X\n \
		Version: %#X\n \
		Entry Point Address: %#X\n \
		Start of program headers: %d\n \
		Start of section headers: %d\n \
		Flags: %#.8X\n \
		Size of this header: %d\n \
		Size of program headers: %d\n \
		Number of program headers: %d\n \
		Size of section headers: %d\n \
		Number of section headers: %d\n \
		Section header string table index: %d\n",
		header->magicNum[0], header->magicNum[1], header->magicNum[2], header->magicNum[3],
		header->bits == 1 ? "ELF64" : "ELF64",
		header->endianness,
		header->version,
		header->abi,
		header->abi_ver,
		change_endian<uint16_t>(header->type),
		change_endian<uint16_t>(header->isa),
		change_endian<uint32_t>(header->version2),
		change_endian<uint64_t>(header->entry),
		change_endian<uint64_t>(header->ph_off),
		change_endian<uint64_t>(header->sh_off),
		change_endian<uint32_t>(header->flags),
		change_endian<uint16_t>(header->header_size),
		change_endian<uint16_t>(header->ph_ent_size),
		change_endian<uint16_t>(header->ph_num),
		change_endian<uint16_t>(header->sh_ent_size),
		change_endian<uint16_t>(header->sh_num),
		change_endian<uint16_t>(header->sh_str_index));
}

void ELF64::printProgramHeaderInfo(int p) {
	printf("%d\t%#x\t%#x\t%#x\t%#x\t%#x\t%x\t%#x\n",
		change_endian<uint32_t>(pHeaders[p].p_type),
		change_endian<uint64_t>(pHeaders[p].p_offset),
		change_endian<uint64_t>(pHeaders[p].p_vaddr),
		change_endian<uint64_t>(pHeaders[p].p_paddr),
		change_endian<uint64_t>(pHeaders[p].p_filesz),
		change_endian<uint64_t>(pHeaders[p].p_memsz),
		change_endian<uint32_t>(pHeaders[p].p_flags),
		change_endian<uint64_t>(pHeaders[p].p_align));
}

void ELF64::printSectionHeaderInfo(int s) {
	printf("%#x\t%d\t%08x\t%x\t%x\t%d\t%8x\t%d\t%d\t%d\n",
		change_endian<uint32_t>(sHeaders[s].s_name),
		change_endian<uint32_t>(sHeaders[s].s_type),
		change_endian<uint64_t>(sHeaders[s].s_addr),
		change_endian<uint64_t>(sHeaders[s].s_offset),
		change_endian<uint64_t>(sHeaders[s].s_size),
		change_endian<uint32_t>(sHeaders[s].s_link),
		change_endian<uint64_t>(sHeaders[s].s_flags),
		change_endian<uint32_t>(sHeaders[s].s_info),
		change_endian<uint64_t>(sHeaders[s].s_addralign),
		change_endian<uint64_t>(sHeaders[s].s_entsize));
}

Endianness ELF64::getEndianness() const {
	return header->endianness == 1 ? LITTLE : BIG;
}


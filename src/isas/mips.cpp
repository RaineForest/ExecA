
#include "mips.h"
#include <cstdlib>
#include <string>

typedef struct {
	int opcode;
	string mnemonic;
	int funct;
	char type;
	bool rs;
	bool rt;
	bool rd;
	bool shamt;
	//bool immed; implied in I-type
	//bool addr; implied in J-type
	bool paren; //parenthetical notation in I-type
} instructionRow;

typedef struct {
	int number;
	string mnemonic;
} registerRow;

static int instruction_callback(void* param, int argc, char** argv, char** azColName) {
	instructionRow* row = reinterpret_cast<instructionRow*>(param);
	row->opcode = atoi(argv[0]);
	row->mnemonic = string(argv[1]);
	row->funct = atoi(argv[2]);
	row->type = argv[3][0];
	row->rs = (bool)atoi(argv[4]);
	row->rt = (bool)atoi(argv[5]);
	row->rd = (bool)atoi(argv[6]);
	row->shamt = (bool)atoi(argv[7]);
	row->paren = (bool)atoi(argv[8]);
	return 0;
}

static int register_callback(void* param, int argc, char** argv, char** azColName) {
	registerRow* row = reinterpret_cast<registerRow*>(param);
	row->number = atoi(argv[0]);
	row->mnemonic = string(argv[1]);
	return 0;
}

static void sqlQuery(sqlite3* db, void* param, string sql, int (*callback)(void*,int,char**,char**)) {
	char* errMsg;
	if(SQLITE_OK != sqlite3_exec(db, sql.c_str(), callback, param, &errMsg)) {
		string msg = string("SQL error: ") + errMsg;
		sqlite3_free(errMsg);
		throw msg;
	}
}

void MIPS::disassemble(uint8_t* data, int len, string* out) {
	sqlite3* db;

	if(sqlite3_open("../../data/mips.db", &db)) {
		string err = string("Cannot open database") + sqlite3_errmsg(db);
		sqlite3_close(db);
		throw err;
	}

	*out = "";

	instructionRow irow;
	string instrQuery = "select * from instruction where opcode = ";
	string regQuery = "select * from register where number = ";
	// each instr is 4 bytes long
	for(int i = 0; i < len; i += 4) {
		uint8_t opcode = data[i+0] >> 2;
		uint8_t rs = (((data[i+0] << 3)) & 0x18) | (data[i+1] >> 5);
		uint8_t rt = data[i+1] & 0x1f;
		uint8_t rd = data[i+2] >> 3;
		uint8_t shamt = (((data[i+2] << 2)) & 0x1c) | (data[i+3] >> 6);
		uint8_t funct = data[i+3] & 0x3f;
		uint16_t immediate = ((uint16_t)data[i+2] << 8) | (uint16_t) data[i+3];
		uint32_t addr = ((uint32_t) (data[i+0] & 0x03) << 24) 
				| (uint32_t) data[i+1] << 16
				| (uint32_t) data[i+2] << 8
				| (uint32_t) data[i+3];
		if(opcode == 0) {
			sqlQuery(db, &irow, 
				instrQuery + to_string(opcode) + " and funct = " 
					+ to_string(funct) + ";", 
				instruction_callback);
		} else {
			sqlQuery(db, &irow, 
				instrQuery + to_string(opcode) + ";", 
				instruction_callback);
		}

		*out += irow.mnemonic + "\t";

		switch(irow.type) {
			case 'R': {
				registerRow rrs, rrt, rrd;
				sqlQuery(db, &rrs, regQuery+to_string(rs)+";", register_callback);
				sqlQuery(db, &rrt, regQuery+to_string(rt)+";", register_callback);
				sqlQuery(db, &rrd, regQuery+to_string(rd)+";", register_callback);
				*out += (irow.rd ? ("$"+rrd.mnemonic + string(", ")) : string(""))
					+ (irow.rs ? ("$"+rrs.mnemonic + string(", ")) : string(""))
					+ (irow.rt ? ("$"+rrt.mnemonic + ((irow.shamt || !irow.rd) ? string(", ") 
						: string(" "))) : string(""))
					+ (irow.shamt ? to_string(shamt) : string(""))
					+ string("\n");
				} break;
			case 'I': {
				registerRow rrs, rrt;
				sqlQuery(db, &rrs, regQuery+to_string(rs)+";", register_callback);
				sqlQuery(db, &rrt, regQuery+to_string(rt)+";", register_callback);
				*out += irow.paren 
					? ((irow.rt ? ("$"+rrt.mnemonic + string(", ")) : string(""))
						+ to_string(immediate) + string("(")
						+ (irow.rs ? "$"+rrs.mnemonic : string("")) + string(")\n"))
					: ((irow.rt ? ("$"+rrt.mnemonic + ", ") : "")
						+ (irow.rs ? "$"+rrs.mnemonic + ", " : "")
						+ to_string(immediate) + "\n");
				} break;
			case 'J': {
				*out += to_string(addr) + "\n";
				} break;
			default: //shouldn't happen
				break;
		}
	}

	sqlite3_close(db);
}

void MIPS::assemble(string asminstr, uint8_t** data, int* len) {

}


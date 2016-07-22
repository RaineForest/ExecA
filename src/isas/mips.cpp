
#include "mips.h"
#include <cstdlib>
#include <string>
#include <sstream>

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

void MIPS::disassemble(uint8_t* data, int len, vector<Instruction>* out, uint64_t offset) {
	sqlite3* db;

	if(sqlite3_open("../../data/mips.db", &db)) {
		string err = string("Cannot open database") + sqlite3_errmsg(db);
		sqlite3_close(db);
		throw err;
	}


	instructionRow irow;
	string instrQuery = "select * from instruction where opcode = ";
	string regQuery = "select * from register where number = ";
	// each instr is 4 bytes long
	for(int i = 0; i < len; i += 4) {
		Instruction instr = Instruction();
		stringstream s;

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

		instr.setAddress(offset + i);
		instr.addToken(irow.mnemonic);
		s << showbase << irow.mnemonic << "\t";

		switch(irow.type) {
			case 'R': {
				registerRow rrs, rrt, rrd;
				sqlQuery(db, &rrs, regQuery+to_string(rs)+";", register_callback);
				sqlQuery(db, &rrt, regQuery+to_string(rt)+";", register_callback);
				sqlQuery(db, &rrd, regQuery+to_string(rd)+";", register_callback);
				s << (irow.rd ? ("$"+rrd.mnemonic + string(", ")) : string(""))
					<< (irow.rs ? ("$"+rrs.mnemonic+string(", ")) : string(""))
					<< (irow.rt ? ("$"+rrt.mnemonic+((irow.shamt||!irow.rd) ? string(", ") 
						: string(" "))) : string(""))
					<< (irow.shamt ? to_string(shamt) : string(""));
				instr.addToken(rrs.mnemonic);
				instr.addToken(rrt.mnemonic);
				instr.addToken(to_string(shamt));
				} break;
			case 'I': {
				registerRow rrs, rrt;
				sqlQuery(db, &rrs, regQuery+to_string(rs)+";", register_callback);
				sqlQuery(db, &rrt, regQuery+to_string(rt)+";", register_callback);
				if(irow.paren) {
					s << (irow.rt ? (string("$")+rrt.mnemonic + string(", ")) : string(""))
						<< immediate 
						<< string("(")
						<< (irow.rs ? string("$")+rrs.mnemonic : string("")) 
						<< string(")");
					instr.addToken(rrt.mnemonic);
					instr.addToken(to_string(immediate));
					instr.addToken(rrs.mnemonic);
				} else {
					instr.addToken(rrt.mnemonic);
					instr.addToken(rrs.mnemonic);
					s << (irow.rt ? (string("$")+rrt.mnemonic + ", ") : "")
						<< (irow.rs ? string("$")+rrs.mnemonic + ", " : "");
					if(irow.mnemonic == "beq" || irow.mnemonic == "bne") {
						s << hex << offset + i + 4 + (immediate << 2) << dec;
						instr.addToken(to_string(offset + i + 4 + (immediate << 2)));
					} else {
						s << hex << immediate << dec;
						instr.addToken(to_string(immediate));
					}
				}
				} break;
			case 'J': {
				s << hex << (addr<<2) + 4 << dec;
				instr.addToken(to_string(addr<<2 + 4));
				} break;
			default: //shouldn't happen
				break;
		}
		
		instr.setText(s.str());
		out->push_back(instr);
	}

	sqlite3_close(db);
}

void MIPS::assemble(vector<Instruction>* asminstr, uint8_t** data, int* len, uint64_t offset) {

}


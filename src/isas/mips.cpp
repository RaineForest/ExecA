
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

	MIPS_Instruction* instrs = (MIPS_Instruction*) data;

	instructionRow irow;
	string instrQuery = "select * from instruction where opcode = ";
	string regQuery = "select * from register where number = ";
	// each instr is 4 bytes long
	for(int i = 0; i < len / 4; i++) {
		if(instrs[i].opcode == 0) {
			sqlQuery(db, &irow, 
				instrQuery + to_string(instrs[i].opcode) + " and funct = " 
					+ to_string(instrs[i].R.funct) + ";", 
				instruction_callback);
		} else {
			sqlQuery(db, &irow, 
				instrQuery + to_string(instrs[i].opcode) + ";", 
				instruction_callback);
		}

		*out += irow.mnemonic + " ";

		switch(irow.type) {
			case 'R': {
				registerRow rs, rt, rd;
				sqlQuery(db, &rs, regQuery+to_string(instrs[i].R.rs) + ";", register_callback);
				sqlQuery(db, &rt, regQuery+to_string(instrs[i].R.rt) + ";", register_callback);
				sqlQuery(db, &rd, regQuery+to_string(instrs[i].R.rd) + ";", register_callback);
				*out += (irow.rd ? (rd.mnemonic + string(", ")) : string(""))
					+ (irow.rs ? (rs.mnemonic + string(", ")) : string(""))
					+ (irow.rt ? (rt.mnemonic + ((irow.shamt || !irow.rd) ? string(", ") 
						: string(" "))) : string(""))
					+ (irow.shamt ? to_string(instrs[i].R.shamt) : string(""))
					+ string("\n");
				} break;
			case 'I': {
				registerRow rs, rt;
				sqlQuery(db, &rs, regQuery+to_string(instrs[i].R.rs) + ";", register_callback);
				sqlQuery(db, &rt, regQuery+to_string(instrs[i].R.rt) + ";", register_callback);
				*out += irow.paren 
					? ((irow.rt ? (rt.mnemonic + string(", ")) : string(""))
						+ to_string(instrs[i].I.immediate) + string("(")
						+ (irow.rs ? rs.mnemonic : string("")) + string(")\n"))
					: ((irow.rt ? (rt.mnemonic + ", ") : "")
						+ (irow.rs ? rs.mnemonic + ", " : "")
						+ to_string(instrs[i].I.immediate) + "\n");
				} break;
			case 'J': {
				*out += to_string(instrs[i].J.address) + "\n";
				} break;
			default: //shouldn't happen
				break;
		}
	}

	sqlite3_close(db);
}

void MIPS::assemble(string asminstr, uint8_t** data, int* len) {

}


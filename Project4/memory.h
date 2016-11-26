#ifndef MEMORY_H_
#define MEMORY_H_

#define MAX_LENGTH_OF_STRING 256
#define MAX_SIZE_OF_TEXT_SEGMENT 100
#define MAX_SIZE_OF_MEMORY_SEGMENT 100
#define TEXT_SEGMENT_BASE_ADDRESS 0x60000000
#define DATA_SEGMENT_BASE_ADDRESS 0x00000000
#define MAX_NUM_INSTRUCTIONS 100
#define NUM_FUS 5
#define NUM_REGISTERS 32
#define NUM_FP_REGISTERS 16
typedef char string[MAX_LENGTH_OF_STRING];


typedef struct text {
	int address;
	string instruction;
	int instruction_code;
	string operands;
	int instruction_type;
} Text;

typedef struct data {
	int address;
	string operands;
	int content;
} Data;

typedef struct Memory {
	int num_instructions;
	Text text_segment[MAX_SIZE_OF_TEXT_SEGMENT];
	Data data_segment[MAX_SIZE_OF_MEMORY_SEGMENT];
} Memory;

typedef struct if_id {
	int ir;
	string operands;
} if_id;

typedef struct id_ex {
	int op_code;
	int rs, rt, rd;
	int op_A, op_B;
	int offset;
	int new_pc;
} id_ex;

typedef struct ex_mem {
	int op_code;
	int alu_out;
	int op_B;
	int rd;
} ex_mem;

typedef struct mem_wb {
	int op_code;
	int mdr;
	int op_B;
	int alu_out;
	int rd;
} mem_wb;

typedef struct op {
	int op_code;
	int op_type;
	int reg_a;
	int reg_b;
	int reg_dest;
} op;

typedef struct pc {
	int program_counter;
	int num_cycles;
	int instruction_count;
	int num_nops;
} pc;

typedef struct i_status {
	string instruction;
	// stores which cycle the operation takes place
	int issue;
	int read;
	int exe;
	int write;
} i_status;

typedef struct fu_status {
	string name;
	int busy; //0 = no, 1 = yes
	int op; // operation code
	int fi, fj, fk; //register address
	int qj, qk; //0 = Integer, 1 = Multiply, 2 = Subtract, 3 = Add, 4 = Load/Store
	int rj, rk; //0 = no, 1 = yes
} fu_status;

typedef struct scoreboard {
	i_status i_status[MAX_NUM_INSTRUCTIONS];
	fu_status fu_status[NUM_FUS];
	int r_status[NUM_REGISTERS];
	int fpr_status[NUM_FP_REGISTERS];
} scoreboard;

#endif // MEMORY_H_

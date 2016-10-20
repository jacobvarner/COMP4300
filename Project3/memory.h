#ifndef MEMORY_H_
#define MEMORY_H_


typedef int int32;

#define MAX_LENGTH_OF_STRING 256
#define MAX_SIZE_OF_TEXT_SEGMENT 100
#define MAX_SIZE_OF_MEMORY_SEGMENT 100
#define TEXT_SEGMENT_BASE_ADDRESS 0x60000000
#define DATA_SEGMENT_BASE_ADDRESS 0x00000000
typedef char string[MAX_LENGTH_OF_STRING];


typedef struct text {
	int32 address;
	string instruction;
	int32 instruction_code;
	string operands;
} Text;

typedef struct data {
	int32 address;
	string operands;
	int32 content;
} Data;

typedef struct Memory {
	Text text_segment[MAX_SIZE_OF_TEXT_SEGMENT];
	Data data_segment[MAX_SIZE_OF_MEMORY_SEGMENT];
} Memory;

typedef struct if_id {
	int32 ir;
}

typedef struct id_ex {
	int32 op_code;
	int32 rs, rt, rd;
	int32 op_A, op_B;
	int32 offset;
	int32 new_pc
}

typedef struct ex_mem {
	int32 op_code;
	int32 alu_out;
	int32 op_B;
	int32 rd;
}

typedef struct mem_wb {
	int32 op_code;
	int32 mdr;
	int32 op_B;
	int32 alu_out;
	int32 rd;
}


#endif // MEMORY_H_

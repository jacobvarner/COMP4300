#ifndef MEMORY_H_
#define MEMORY_H_


typedef int int32;

#define MAX_LENGTH_OF_STRING 16
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


#endif // MEMORY_H_

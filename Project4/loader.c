#include "loader.h"
#include "memory.h"
#include <string.h>
#include <stdio.h>

typedef int segment;
enum {text, data};

#define SIZE_OF_BUFFER 255
segment currentSegment;

Memory load_program(char program_file_path[]) {
	FILE *program_file_ptr = fopen(program_file_path, "r");
	char buff[SIZE_OF_BUFFER];
	char line[SIZE_OF_BUFFER];
	Memory code_segment;
	int counter = 0;
	int text_segment_index = 0;
	int data_segment_index = 0;
	int nextTextSegmentAddress = TEXT_SEGMENT_BASE_ADDRESS;
	int nextDataSegmentAddress = DATA_SEGMENT_BASE_ADDRESS;
	while(fgets(line, SIZE_OF_BUFFER, program_file_ptr) != NULL) {
		//printf("Loop start \n");
		if (line[0] == '#') {
		  continue;
	  } else if(line[0] == '.') {
			setCurrentSegment(line);
		} else if(currentSegment == text) {
			if (line[0] == '\n') {
				printf("\n");
				continue;
			}
			code_segment.text_segment[text_segment_index].address = nextTextSegmentAddress;
			nextTextSegmentAddress++;
			sscanf(line, "%s %[^\n]", code_segment.text_segment[text_segment_index].instruction, code_segment.text_segment[text_segment_index].operands);
			code_segment.text_segment[text_segment_index].instruction_code = assignInstructionCode(code_segment.text_segment[text_segment_index].instruction);
			code_segment.text_segment[text_segment_index].instruction_type = assignInstructionType(code_segment.text_segment[text_segment_index].instruction_code);
			text_segment_index++;
			counter++;
		} else if(currentSegment == data) { // In Data Segment
			if (line[0] == '\n') {
				printf("\n");
				continue;
			}
			code_segment.data_segment[data_segment_index].address = nextDataSegmentAddress;
			nextDataSegmentAddress++;
			sscanf(line, "%s %d", code_segment.data_segment[data_segment_index].operands, &code_segment.data_segment[data_segment_index].content);
			data_segment_index++;
		}
		printf("%s", line);
		if (line[0] == 'E') {
			break;
		}
	}
	fclose(program_file_ptr);
	code_segment.num_instructions = counter;
	return code_segment;
}

void setCurrentSegment(char line[]) {
	if(line[1] == 't') {
		currentSegment = text;
	} else if(line[1] == 'd') {
		currentSegment = data;
	}
}

int assignInstructionCode(string instruction) {
	int output;
	if (strcmp(instruction, "nop") == 0) {
		output = 0;
	} else if (strcmp(instruction, "add") == 0) {
		output = 1;
	} else if (strcmp(instruction, "addi") == 0) {
		output = 2;
	} else if (strcmp(instruction, "b") == 0) {
		output = 3;
	} else if (strcmp(instruction, "beqz") == 0) {
		output = 4;
	} else if (strcmp(instruction, "bge") == 0) {
		output = 5;
	} else if (strcmp(instruction, "bne") == 0) {
		output = 6;
	} else if (strcmp(instruction, "la") == 0) {
		output = 7;
	} else if (strcmp(instruction, "lb") == 0) {
		output = 8;
	} else if (strcmp(instruction, "li") == 0) {
		output = 9;
	} else if (strcmp(instruction, "subi") == 0) {
		output = 10;
	} else if (strcmp(instruction, "syscall") == 0) {
		output = 11;
	} else if (strcmp(instruction, "fadd") == 0) {
		output = 12;
	} else if (strcmp(instruction, "fmul") == 0) {
		output = 13;
	} else if (strcmp(instruction, "fsub") == 0) {
		output = 14;
	} else if (strcmp(instruction, "l.d") == 0) {
		output = 15;
	} else if (strcmp(instruction, "s.d") == 0) {
		output = 16;
	} else {
		printf("Not an instruction");
	}

	return output;
}

int assignInstructionType(int instruction_code) {
	int output;
	switch (instruction_code) {
		case 0:
			output = -1;
			break;
		case 1:
			output = 3;
			break;
		case 2:
			output = 3;
			break;
		case 3:
			output = 0;
			break;
		case 4:
			output = 0;
			break;
		case 5:
			output = 0;
			break;
		case 6:
			output = 0;
			break;
		case 7:
			output = 4;
			break;
		case 8:
			output = 4;
			break;
		case 9:
			output = 4;
			break;
		case 10:
			output = 2;
			break;
		case 11:
			output = -1;
			break;
		case 12:
			output = 3;
			break;
		case 13:
			output = 1;
			break;
		case 14:
			output = 2;
			break;
		case 15:
			output = 4;
			break;
		case 16:
			output = 4;
			break;
	}
	return output;
}

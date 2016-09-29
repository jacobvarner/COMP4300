#include "loader.h"
#include <string.h>
#include <stdio.h>

typedef int segment;
enum {text, data};

#define SIZE_OF_BUFFER 255
segment currentSegment;

Memory loadProgramIntoMemory(char program_file_path[]) {
	FILE *program_file_ptr = fopen(program_file_path, "r");
	char buff[SIZE_OF_BUFFER];
	char line[SIZE_OF_BUFFER];
	Memory memory;
	int counter = 0;
	int32 text_segment_index = 0;
	int32 data_segment_index = 0;
	int32 nextTextSegmentAddress = TEXT_SEGMENT_BASE_ADDRESS;
	int32 nextDataSegmentAddress = DATA_SEGMENT_BASE_ADDRESS;
	while(fgets(line, SIZE_OF_BUFFER, program_file_ptr) != NULL) {
		//printf("Loop start \n");
		if(line[0] == '.') {
			setCurrentSegment(line);
		} else if(currentSegment == text) {
			if (line[0] == '\n') {
				printf("\n");
				continue;
			}
			memory.text_segment[text_segment_index].address = nextTextSegmentAddress;
			nextTextSegmentAddress++;
			sscanf(line, "%s %s", memory.text_segment[text_segment_index].instruction, memory.text_segment[text_segment_index].operand);
			//printf("Instruction code should be: %s\n", memory.text_segment[text_segment_index].instruction);
			memory.text_segment[text_segment_index].instruction_code = assignInstructionCode(memory.text_segment[text_segment_index].instruction);
			//printf("Instruction code assigned as: %d\n", memory.text_segment[text_segment_index].instruction_code);
			text_segment_index++;
		} else if(currentSegment == data) { // In Data Segment
			if (line[0] == '\n') {
				printf("\n");
				continue;
			}
			memory.data_segment[data_segment_index].address = nextDataSegmentAddress;
			nextDataSegmentAddress++;
			sscanf(line, "%s %d", memory.data_segment[data_segment_index].operand, &memory.data_segment[data_segment_index].content);
			data_segment_index++;
		}
		printf("%s", line);
		if (line[0] == 'E') {
			break;
		}
	}
	fclose(program_file_ptr);
	return memory;
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
	if (strcmp(instruction, "PUSH") == 0 || strcmp(instruction, "LOAD") == 0) {
		output = 0;
	} else if (strcmp(instruction, "POP") == 0 || strcmp(instruction, "STO") == 0) {
		output = 1;
	} else if (strcmp(instruction, "ADD") == 0) {
		output = 2;
	} else if (strcmp(instruction, "MULT") == 0) {
		output = 3;
	} else if (strcmp(instruction, "END") == 0) {
		output = 4;
	} else if (strcmp(instruction, "ADDI") == 0) {
		output = 5
	} else if (strcmp(instruction, "B") == 0) {
		output = 6
	} else if (strcmp(instruction, "BEQZ") == 0) {
		output = 7
	} else if (strcmp(instruction, "BGE") == 0) {
		output = 8
	} else if (strcmp(instruction, "BNE") == 0) {
		output = 9
	} else if (strcmp(instruction, "LA") == 0) {
		output = 10
	} else if (strcmp(instruction, "LB") == 0) {
		output = 11
	} else if (strcmp(instruction, "LI") == 0) {
		output = 12
	} else if (strcmp(instruction, "SUBI") == 0) {
		output = 13
	} else if (strcmp(instruction, "SYSCALL") == 0) {
		output = 14
	} else {
		printf("Not an instruction!");
	}

	return output;
}

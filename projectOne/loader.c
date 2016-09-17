#include "loader.h"
#include <string.h>
#include <stdio.h>

typedef int segment;
enum {text, data};

#define SIZE_OF_BUFFER 255
segment currentSegment;

Memory loadProgramIntoMemory(char program_file_path[]) {
	FILE *program_file_ptr = fopen(program_file_path, "rt");
	char buff[SIZE_OF_BUFFER];
	char line[SIZE_OF_BUFFER];
	Memory memory;
	int counter = 0;
	int32 text_segment_index = 0;
	int32 data_segment_index = 0;
	int32 nextTextSegmentAddress = TEXT_SEGMENT_BASE_ADDRESS;
	int32 nextDataSegmentAddress = DATA_SEGMENT_BASE_ADDRESS;
	while(fgets(line, SIZE_OF_BUFFER, program_file_ptr) != NULL) {
		/* printf("Loop start \n");
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
			memory.text_segment[text_segment_index].instruction_code = assignInstructionCode(memory.text_segment[text_segment_index].instruction);
			printf("Instruction code assigned \n");
			text_segment_index++;
		} else if(currentSegment == data) { // In Data Segment
			if (line[0] == '\n') {
				printf("\n");
				continue;
			}
			memory.data_segment[data_segment_index].address = nextDataSegmentAddress;
			nextDataSegmentAddress++;
			sscanf(line, "%d %d", &memory.data_segment[data_segment_index].address, &memory.data_segment[data_segment_index].content);
			data_segment_index++;
		}
		printf("%d - %s", counter, line);
		counter++;
		if (counter == 20) {
			printf("WHY WON\'T THIS PROGRAM END?");
		}
		if (line[0] == 'E') {
			printf("File has reached the end.");
			break;
		} else {
			printf("Continuing...");
		} */
	}
	printf("Out of the loop");
	//fclose(program_file_ptr);
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
	if (strcmp(instruction, "PUSH") == 0) {
		output = 0;
	} else if (strcmp(instruction, "POP") == 0) {
		output = 1;
	} else if (strcmp(instruction, "ADD") == 0) {
		output = 2;
	} else if (strcmp(instruction, "MULT") == 0) {
		output = 3;
	} else if (strcmp(instruction, "END") == 0) {
		output = 4;
	} else {
		printf("Not an instruction!");
	}

	return output;
}

#include <stdlib.h>
#include <stdio.h>
#include "loader.h"
#include "memory.h"
#include "read.h"

Memory memory;
int accumulator;

void load(string operand);
void sto(string operand);
void add(string operand);
void multiply(string operand);

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("You must provide an assembly file to be read.");
    return 1; /* Error: Must provide assembly file to be read. */
  }
	printf("Loading into memory...\n\n");
	memory = loadProgramIntoMemory(argv[1]);
	printf("\nLoading complete!\n\n");

	int program_counter = 0;
	int run = 1;
	Text currentText;
	int currentInstructionCode;
	while (run) {
		currentText = loadText(program_counter + TEXT_SEGMENT_BASE_ADDRESS, memory);
		//printf("%s \n", currentText.instruction);
		currentInstructionCode = currentText.instruction_code;
		switch (currentInstructionCode) {
			case 0 : //LOAD
				load(currentText.operand);
				break;
			case 1 : //STO
				sto(currentText.operand);
				break;
			case 2 : //ADD
				add(currentText.operand);
				break;
			case 3 : //MULT
				multiply(currentText.operand);
				break;
			case 4 : //END
				run = 0;
				break;
			default :
				run = 0;
		}
		program_counter++;
	}

	printf("\nRESULTS:\n\nAccumulator: %d\n\n.data\n  %s %d\n  %s %d\n  %s %d\n  %s %d\n  %s %d\n\n", accumulator,
	memory.data_segment[0].operand, memory.data_segment[0].content,
	memory.data_segment[1].operand, memory.data_segment[1].content,
	memory.data_segment[2].operand, memory.data_segment[2].content,
	memory.data_segment[3].operand, memory.data_segment[3].content,
	memory.data_segment[4].operand, memory.data_segment[4].content);

	return 0;
}

void load(string operand) {
	//fetch value from data section using operand
	int32 address = (int32)strtol(operand, NULL, 0);
	Data data = loadData(address, memory);
	//store that value at the top of stack
	accumulator = data.content;
  printf("\nStatus:\n\nAccumulator: %d\n\n.data\n  %s %d\n  %s %d\n  %s %d\n  %s %d\n  %s %d\n\n", accumulator,
	memory.data_segment[0].operand, memory.data_segment[0].content,
	memory.data_segment[1].operand, memory.data_segment[1].content,
	memory.data_segment[2].operand, memory.data_segment[2].content,
	memory.data_segment[3].operand, memory.data_segment[3].content,
	memory.data_segment[4].operand, memory.data_segment[4].content);
}

void sto(string operand) {
	//fetch value from data section using operand
	int32 address = (int32)strtol(operand, NULL, 0);
	//store that value at the top of stack
	memory.data_segment[address].content = accumulator;
  printf("\nStatus:\nAccumulator: %d\n\n.data\n  %s %d\n  %s %d\n  %s %d\n  %s %d\n  %s %d\n\n", accumulator,
	memory.data_segment[0].operand, memory.data_segment[0].content,
	memory.data_segment[1].operand, memory.data_segment[1].content,
	memory.data_segment[2].operand, memory.data_segment[2].content,
	memory.data_segment[3].operand, memory.data_segment[3].content,
	memory.data_segment[4].operand, memory.data_segment[4].content);
}

void add(string operand) {
	int sum = 0;
  int32 address = (int32)strtol(operand, NULL, 0);
	Data data = loadData(address, memory);
  sum += data.content;
  sum += accumulator;
  accumulator = sum;
  printf("\nStatus:\nAccumulator: %d\n\n.data\n  %s %d\n  %s %d\n  %s %d\n  %s %d\n  %s %d\n\n", accumulator,
	memory.data_segment[0].operand, memory.data_segment[0].content,
	memory.data_segment[1].operand, memory.data_segment[1].content,
	memory.data_segment[2].operand, memory.data_segment[2].content,
	memory.data_segment[3].operand, memory.data_segment[3].content,
	memory.data_segment[4].operand, memory.data_segment[4].content);
}

void multiply(string operand) {
	int product = 0;
  int32 address = (int32)strtol(operand, NULL, 0);
	Data data = loadData(address, memory);
  product = data.content * accumulator;
  accumulator = product;
  printf("\nStatus:\nAccumulator: %d\n\n.data\n  %s %d\n  %s %d\n  %s %d\n  %s %d\n  %s %d\n\n", accumulator,
	memory.data_segment[0].operand, memory.data_segment[0].content,
	memory.data_segment[1].operand, memory.data_segment[1].content,
	memory.data_segment[2].operand, memory.data_segment[2].content,
	memory.data_segment[3].operand, memory.data_segment[3].content,
	memory.data_segment[4].operand, memory.data_segment[4].content);
}

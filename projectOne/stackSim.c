#include <stdlib.h>
#include <stdio.h>
#include "loader.h"
#include "memory.h"
#include "read.h"

#define STACK_SIZE 50

typedef struct stack {
	int values[STACK_SIZE];
	int top;
} Stack;

void push(string operand, Stack stack);
string* pop(int address, Stack stack);
void add(Stack stack);
void multiply(Stack stack);

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("You must provide an assembly file to be read.");
    return 1; /* Error: Must provide assembly file to be read. */
  }
	printf("Before memory");
	Memory memory = loadProgramIntoMemory(argv[1]);
	printf("After memory");

	printf("Before");
	int32 address = atoi(memory.text_segment[0].operand);
	printf("Adress conversion: %d", address);

	printf("After");

	Stack stack;
	stack.top = -1;

	//printf("\n");
	//printf("Address 0: %d, Instruction 0: %s, Operand 0: %s \n", memory.text_segment[0].address, memory.text_segment[0].instruction, memory.text_segment[0].operand);
	//printf("Address 1: %d, Instruction 1: %s, Operand 1: %s \n", memory.text_segment[1].address, memory.text_segment[1].instruction, memory.text_segment[1].operand);
	//printf("Address 2: %d, Instruction 2: %s, Operand 2: %s \n", memory.text_segment[2].address, memory.text_segment[2].instruction, memory.text_segment[2].operand);
	//printf("Address 3: %d, Instruction 3: %s, Operand 3: %s \n", memory.text_segment[3].address, memory.text_segment[3].instruction, memory.text_segment[3].operand);

	int program_counter = 0;
	int run = 1;
	Text currentText;
	int currentInstructionCode;
	while (run) {
		currentText = loadText(program_counter + TEXT_SEGMENT_BASE_ADDRESS, memory);
		currentInstructionCode = currentText.instruction_code;
		switch (currentInstructionCode) {
			case 0 : //PUSH
				push(currentText.operand, stack);
				break;
			case 1 : //POP
				pop(currentText.address, stack);
				break;
			case 2 : //ADD
				add(stack);
				break;
			case 3 : //MULT
				multiply(stack);
				break;
			case 4 : //END
				run = 0;
				break;
			default :
				printf("This is not an option. LOL");
		}
	}

	return 0;
}

void push(string operand, Stack stack) {
	//fetch value from data section using operand
	//int32 address = atoi(operand);
	//printf("%d", address);
	//Data data = loadData()
	//store that value at the top of stack
}

string* pop(int address, Stack stack) {

}

void add(Stack stack) {

}

void multiply(Stack stack) {

}

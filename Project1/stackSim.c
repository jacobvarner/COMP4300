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

Stack stack;
Memory memory;

void push(string operand);
void pop(string operand);
void add();
void multiply();

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("You must provide an assembly file to be read.");
    return 1; /* Error: Must provide assembly file to be read. */
  }
	printf("Loading into memory...\n\n");
	memory = loadProgramIntoMemory(argv[1]);
	printf("\nLoading complete!\n\n");

	stack.top = -1;

	int program_counter = 0;
	int run = 1;
	Text currentText;
	int currentInstructionCode;
	while (run) {
		currentText = loadText(program_counter + TEXT_SEGMENT_BASE_ADDRESS, memory);
		//printf("%s \n", currentText.instruction);
		currentInstructionCode = currentText.instruction_code;
		switch (currentInstructionCode) {
			case 0 : //PUSH
				push(currentText.operand);
				break;
			case 1 : //POP
				pop(currentText.operand);
				break;
			case 2 : //ADD
				add();
				break;
			case 3 : //MULT
				multiply();
				break;
			case 4 : //END
				run = 0;
				break;
			default :
				run = 0;
		}
		program_counter++;
	}

	printf("\nRESULTS:\n.data\n  %s %d\n  %s %d\n  %s %d\n  %s %d\n  %s %d\n\n",
	memory.data_segment[0].operand, memory.data_segment[0].content,
	memory.data_segment[1].operand, memory.data_segment[1].content,
	memory.data_segment[2].operand, memory.data_segment[2].content,
	memory.data_segment[3].operand, memory.data_segment[3].content,
	memory.data_segment[4].operand, memory.data_segment[4].content);

	return 0;
}

void push(string operand) {
	//fetch value from data section using operand
	int32 address = (int32)strtol(operand, NULL, 0);
	Data data = loadData(address, memory);
	//store that value at the top of stack
	stack.values[stack.top + 1] = data.content;
	stack.top++;
	printf("Stack: bottom -> %d %d %d %d %d %d %d <- top\n", stack.values[0], stack.values[1], stack.values[2], stack.values[3], stack.values[4], stack.values[5], stack.values[6]);
}

void pop(string operand) {
	//fetch value from data section using operand
	int32 address = (int32)strtol(operand, NULL, 0);
	//store that value at the top of stack
	memory.data_segment[address].content = stack.values[stack.top];
	stack.values[stack.top] = 0;
	stack.top--;
	printf("Stack: bottom -> %d %d %d %d %d %d %d <- top\n", stack.values[0], stack.values[1], stack.values[2], stack.values[3], stack.values[4], stack.values[5], stack.values[6]);
}

void add() {
	int sum = 0;
	sum += stack.values[stack.top - 1];
	sum += stack.values[stack.top];
	stack.values[stack.top] = 0;
	stack.top--;
	stack.values[stack.top] = sum;
	printf("Stack: bottom -> %d %d %d %d %d %d %d <- top\n", stack.values[0], stack.values[1], stack.values[2], stack.values[3], stack.values[4], stack.values[5], stack.values[6]);
}

void multiply() {
	int product = 0;
	product = stack.values[stack.top - 1] * stack.values[stack.top];
	stack.values[stack.top] = 0;
	stack.top--;
	stack.values[stack.top] = product;
	printf("Stack: bottom -> %d %d %d %d %d %d %d <- top\n", stack.values[0], stack.values[1], stack.values[2], stack.values[3], stack.values[4], stack.values[5], stack.values[6]);
}

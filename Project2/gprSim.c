#include <stdlib.h>
#include <stdio.h>
#include "loader.h"
#include "memory.h"
#include "read.h"

#define NUM_REGISTERS 32;

Memory memory;
int registers[NUM_REGISTERS];

void load(string operands);
void sto(string operands);
void add(string operands);
void multiply(string operands);
void addi(string operands);
void b(string operands);
void beqz(string operands);
void bge(string operands);
void bne(string operands);
void la(string operands);
void lb(string operands);
void li(string operands);
void subi(string operands);
void syscall(string operands);

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
				load(currentText.operands);
				break;
			case 1 : //STO
				sto(currentText.operands);
				break;
			case 2 : //ADD
				add(currentText.operands);
				break;
			case 3 : //MULT
				multiply(currentText.operands);
				break;
			case 4 : //END
				run = 0;
				break;
      case 5 : //ADDI
				addi(currentText.operands)
				break;
      case 6 : //B
				b(currentText.operands)
				break;
      case 7 : //BEQZ
				beqz(currentText.operands)
				break;
      case 8 : //BGE
				bge(currentText.operands)
				break;
      case 9 : //BNE
				bne(currentText.operands)
				break;
      case 10 : //LA
				la(currentText.operands)
				break;
      case 11 : //LB
				lb(currentText.operands)
				break;
      case 12 : //LI
				li(currentText.operands)
				break;
      case 13 : //SUBI
				subi(currentText.operands)
				break;
      case 14 : //SYSCALL
				syscall(currentText.operands)
				break;
			default :
				run = 0;
		}
		program_counter++;
	}

	printf("\nRESULTS:\n\nRegister[0]: %d\n\n.data\n  %s %d\n  %s %d\n  %s %d\n  %s %d\n  %s %d\n\n", registers[0],
	memory.data_segment[0].operands, memory.data_segment[0].content,
	memory.data_segment[1].operands, memory.data_segment[1].content,
	memory.data_segment[2].operands, memory.data_segment[2].content,
	memory.data_segment[3].operands, memory.data_segment[3].content,
	memory.data_segment[4].operands, memory.data_segment[4].content);

	return 0;
}

void load(string operands) {
	//fetch value from data section using operands
	int32 address = (int32)strtol(operands, NULL, 0);
	Data data = loadData(address, memory);
	//store that value at the top of stack
	registers[0] = data.content;
  printf("\nStatus:\n\nRegister[0]: %d\n\n.data\n  %s %d\n  %s %d\n  %s %d\n  %s %d\n  %s %d\n\n", registers[0],
	memory.data_segment[0].operands, memory.data_segment[0].content,
	memory.data_segment[1].operands, memory.data_segment[1].content,
	memory.data_segment[2].operands, memory.data_segment[2].content,
	memory.data_segment[3].operands, memory.data_segment[3].content,
	memory.data_segment[4].operands, memory.data_segment[4].content);
}

void sto(string operands) {
	//fetch value from data section using operands
	int32 address = (int32)strtol(operands, NULL, 0);
	//store that value at the top of stack
	memory.data_segment[address].content = registers[0];
  printf("\nStatus:\nRegister[0]: %d\n\n.data\n  %s %d\n  %s %d\n  %s %d\n  %s %d\n  %s %d\n\n", registers[0],
	memory.data_segment[0].operands, memory.data_segment[0].content,
	memory.data_segment[1].operands, memory.data_segment[1].content,
	memory.data_segment[2].operands, memory.data_segment[2].content,
	memory.data_segment[3].operands, memory.data_segment[3].content,
	memory.data_segment[4].operands, memory.data_segment[4].content);
}

void add(string operands) {
	int sum = 0;
  int32 address = (int32)strtol(operands, NULL, 0);
	Data data = loadData(address, memory);
  sum += data.content;
  sum += registers[0];
  registers[0] = sum;
  printf("\nStatus:\nRegister[0]: %d\n\n.data\n  %s %d\n  %s %d\n  %s %d\n  %s %d\n  %s %d\n\n", registers[0],
	memory.data_segment[0].operands, memory.data_segment[0].content,
	memory.data_segment[1].operands, memory.data_segment[1].content,
	memory.data_segment[2].operands, memory.data_segment[2].content,
	memory.data_segment[3].operands, memory.data_segment[3].content,
	memory.data_segment[4].operands, memory.data_segment[4].content);
}

void multiply(string operands) {
	int product = 0;
  int32 address = (int32)strtol(operands, NULL, 0);
	Data data = loadData(address, memory);
  product = data.content * registers[0];
  registers[0] = product;
  printf("\nStatus:\nRegister[0]: %d\n\n.data\n  %s %d\n  %s %d\n  %s %d\n  %s %d\n  %s %d\n\n", registers[0],
	memory.data_segment[0].operands, memory.data_segment[0].content,
	memory.data_segment[1].operands, memory.data_segment[1].content,
	memory.data_segment[2].operands, memory.data_segment[2].content,
	memory.data_segment[3].operands, memory.data_segment[3].content,
	memory.data_segment[4].operands, memory.data_segment[4].content);
}

void addi(string operands) {
  int rdest;
  int rsrc1;
  int imm;
  sscanf(operands, "%*c%d %*c%d %d", rdest, rsrc1, imm);
  int answer = registers[rsrc1] + imm;
  registers[rdest] = answer;
}

void b() {

}

void beqz() {

}

void bge() {

}

void bne() {

}

void la() {

}

void lb() {

}

void li() {

}

void subi(string operands) {
	int32 rdest, rsrc1, imm;
	sscanf(operands, "%*c%d %*c%d %d", &rdest, &rsrc1, &imm);
	registers[rdest] = registers[rsrc1] - registers[imm];
}

void syscall() {

}

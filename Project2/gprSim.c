#include <stdlib.h>
#include <stdio.h>
#include "loader.h"
#include "memory.h"
#include "read.h"

#define NUM_REGISTERS 32;

Memory memory;
int registers[NUM_REGISTERS];

void load(string operand);
void sto(string operand);
void add(string operand);
void multiply(string operand);
void addi();
void b();
void beqz();
void bge();
void bne();
void la();
void lb();
void li();
void subi();
void syscall();

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
      case 5 : //ADDI
				addi()
				break;
      case 6 : //B
				b()
				break;
      case 7 : //BEQZ
				beqz()
				break;
      case 8 : //BGE
				bge()
				break;
      case 9 : //BNE
				bne()
				break;
      case 10 : //LA
				la()
				break;
      case 11 : //LB
				lb()
				break;
      case 12 : //LI
				li()
				break;
      case 13 : //SUBI
				subi()
				break;
      case 14 : //SYSCALL
				syscall()
				break;
			default :
				run = 0;
		}
		program_counter++;
	}

	printf("\nRESULTS:\n\nRegister[0]: %d\n\n.data\n  %s %d\n  %s %d\n  %s %d\n  %s %d\n  %s %d\n\n", registers[0],
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
	registers[0] = data.content;
  printf("\nStatus:\n\nRegister[0]: %d\n\n.data\n  %s %d\n  %s %d\n  %s %d\n  %s %d\n  %s %d\n\n", registers[0],
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
	memory.data_segment[address].content = registers[0];
  printf("\nStatus:\nRegister[0]: %d\n\n.data\n  %s %d\n  %s %d\n  %s %d\n  %s %d\n  %s %d\n\n", registers[0],
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
  sum += registers[0];
  registers[0] = sum;
  printf("\nStatus:\nRegister[0]: %d\n\n.data\n  %s %d\n  %s %d\n  %s %d\n  %s %d\n  %s %d\n\n", registers[0],
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
  product = data.content * registers[0];
  registers[0] = product;
  printf("\nStatus:\nRegister[0]: %d\n\n.data\n  %s %d\n  %s %d\n  %s %d\n  %s %d\n  %s %d\n\n", registers[0],
	memory.data_segment[0].operand, memory.data_segment[0].content,
	memory.data_segment[1].operand, memory.data_segment[1].content,
	memory.data_segment[2].operand, memory.data_segment[2].content,
	memory.data_segment[3].operand, memory.data_segment[3].content,
	memory.data_segment[4].operand, memory.data_segment[4].content);
}

void addi() {

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

void subi() {

}

void syscall() {

}

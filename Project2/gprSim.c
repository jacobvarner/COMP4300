#include <stdlib.h>
#include <stdio.h>
#include "loader.h"
#include "memory.h"
#include "read.h"

#define NUM_REGISTERS 32;

Memory memory;
int program_counter;
int num_cycles;
int instruction_count;
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

	program_counter = 0;
  num_cycles = 0;
  instruction_count = 0;
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

  double speedup = (8 * instruction_count) / num_cycles;

  // Output summary
	printf("\nRESULTS:\n\nRegister[0]: %d\nRegister[1]: %d\nRegister[2]: %d\n" +
  "Register[3]: %d\nRegister[4]: %d\nRegister[5]: %d\nRegister[6]: %d\n" +
  "Register[7]: %d\nRegister[8]: %d\nRegister[9]: %d\nRegister[10]: %d\n" +
  "Register[11]: %d\nRegister[12]: %d\nRegister[13]: %d\nRegister[14]: %d\n" +
  "Register[15]: %d\nRegister[16]: %d\nRegister[17]: %d\nRegister[18]: %d\n" +
  "Register[19]: %d\nRegister[20]: %d\nRegister[21]: %d\nRegister[22]: %d\n" +
  "Register[23]: %d\nRegister[24]: %d\nRegister[25]: %d\nRegister[26]: %d\n" +
  "Register[27]: %d\nRegister[28]: %d\nRegister[29]: %d\nRegister[30]: %d\n" +
  "Register[31]: %d\n\nC = %d\nIC = %d\n[8*IC]/C = %d\n\n", registers[0],
  registers[1], registers[2], registers[3], registers[4], registers[5],
  registers[6], registers[7], registers[8], registers[9], registers[10],
  registers[11], registers[12], registers[13], registers[14], registers[15],
  registers[16], registers[17], registers[18], registers[19], registers[20],
  registers[21], registers[22], registers[23], registers[24], registers[25],
  registers[26], registers[27], registers[28], registers[29], registers[30],
  registers[31], num_cycles, instruction_count, speedup);

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
  int32 rdest;
  int32 rsrc1;
  int32 imm;
  sscanf(operands, "%*c%d %*c%d %d", &rdest, &rsrc1, &imm);
  int32 answer = registers[rsrc1] + imm;
  registers[rdest] = answer;
  num_cycles += 6;
  instruction_count++;
}

void b(string operands) {
	int32 label;
	sscanf(operands, "%d", label);
	program_counter += label;
  num_cycles += 4;
  instruction_count++;
}

void beqz(string operands) {
	int32 label, rsrc1;
	sscanf(operands, "%*c%d, %d", rsrc1, label);
	if(registers[rsrc1] == 0) {
		program_counter += label;
	}
  num_cycles += 5;
  instruction_count++;
}

void bge(string operands) {
	int32 label, rsrc1, rsrc2;
	sscanf(operands, "%*c%d, %d", rsrc1, rsrc2, label);
	if(registers[rsrc1] >= registers[rsrc2]) {
		program_counter += label;
	}
  num_cycles += 5;
  instruction_count++;
}

void bne(string operands) {
	int32 label, rsrc1, rsrc2;
	sscanf(operands, "%*c%d, %d", rsrc1, rsrc2, label);
	if(registers[rsrc1] != registers[rsrc2]) {
		program_counter += label;
	}
  num_cycles += 5;
  instruction_count++;
}

void la(string operands) {
  int32 rdest;
  int32 label;
  sscanf(operands, "%*c%d %d", &rdest, &label);
  registers[rdest] = label;
  num_cycles += 5;
  instruction_count++;
}

void lb(string operands) {
  int32 rdest;
  int32 offset;
  int32 rsrc1;
  sscanf(operands, "%*c%d %d %*c%d", &rdest, &offset, &rsrc1);
  registers[rdest] = registers[rsrc1 + offset];
  num_cycles += 6;
  instruction_count++;
}

void li(string operands) {
  int32 rdest;
  int32 imm;
  sscanf(operands, "%*c%d %d", &rdest, &imm);
  registers[rdest] = imm;
  num_cycles += 3;
  instruction_count++;
}

void subi(string operands) {
	int32 rdest, rsrc1, imm;
	sscanf(operands, "%*c%d %*c%d %d", &rdest, &rsrc1, &imm);
	registers[rdest] = registers[rsrc1] - registers[imm];
  num_cycles += 6;
  instruction_count++;
}

void syscall(string operands) {
  num_cycles += 8;
  instruction_count++;
}

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "loader.h"
#include "memory.h"
#include "read.h"

#define NUM_REGISTERS 32
#define NUM_STRINGS 3

Memory memory;
int program_counter;
int num_cycles;
int instruction_count;
int run;
int registers[NUM_REGISTERS];
string strings[NUM_STRINGS];

// $29 - syscall parameters / return values
// $30 - syscall parameters
// $31 - syscall parameters

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
void syscall();
void readString();
void writeString();

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("You must provide an assembly file to be read.");
    return 1; /* Error: Must provide assembly file to be read. */
  }
	printf("Loading into memory...\n\n");
	memory = loadProgramIntoMemory(argv[1]);
	printf("\nLoading complete!\n\n");

  strcpy(strings[0], argv[2]);
  strcpy(strings[1], "This is a palindrome :)");
  strcpy(strings[2], "This is not a palindrome :(");

	program_counter = 0;
  num_cycles = 0;
  instruction_count = 0;
	run = 1;
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
				addi(currentText.operands);
				break;
      case 6 : //B
				b(currentText.operands);
				break;
      case 7 : //BEQZ
				beqz(currentText.operands);
				break;
      case 8 : //BGE
				bge(currentText.operands);
				break;
      case 9 : //BNE
				bne(currentText.operands);
				break;
      case 10 : //LA
				la(currentText.operands);
				break;
      case 11 : //LB
				lb(currentText.operands);
				break;
      case 12 : //LI
				li(currentText.operands);
				break;
      case 13 : //SUBI
				subi(currentText.operands);
				break;
      case 14 : //SYSCALL
				syscall();
				break;
			default :
				run = 0;
		}
		program_counter++;
	}

  double speedup = (8 * (double)instruction_count) / (double)num_cycles;

  // Output summary
	printf("\nRESULTS:\n\nRegister[0]: %d\nRegister[1]: %d\nRegister[2]: %d\n\
Register[3]: %d\nRegister[29]: %d\nRegister[30]: %d\n\
Register[31]: %d\n\nC = %d\nIC = %d\n[8*IC]/C = %f\n\n", registers[0],
  registers[1], registers[2], registers[3], registers[29], registers[30],
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
  //printf("ADDI: Destination: %d, Source: %d, Imm: %d\n", rdest, rsrc1, imm);
  int32 answer = registers[rsrc1] + imm;
  //printf("Answer: %d\n", answer);
  registers[rdest] = answer;
  num_cycles += 6;
  instruction_count++;
}

void b(string operands) {
	int32 label;
	sscanf(operands, "%d", &label);
  //printf("B: Label: %d\n", label);
	program_counter += label;
  num_cycles += 4;
  instruction_count++;
}

void beqz(string operands) {
	int32 label, rsrc1;
	sscanf(operands, "%*c%d %d", &rsrc1, &label);
  //printf("BEQZ: Register[%d]: %d\n", rsrc1, registers[rsrc1]);
	if(registers[rsrc1] == 0) {
		program_counter += label;
	}
  num_cycles += 5;
  instruction_count++;
}

void bge(string operands) {
	int32 label, rsrc1, rsrc2;
	sscanf(operands, "%*c%d %*c%d %d", &rsrc1, &rsrc2, &label);
  //printf("BGE: Source1: %d = %d, Source2: %d = %d, Label: %d\n", rsrc1, registers[rsrc1], rsrc2, registers[rsrc2], label);
	if(registers[rsrc1] >= registers[rsrc2]) {
		program_counter += label;
	}
  num_cycles += 5;
  instruction_count++;
}

void bne(string operands) {
	int32 label, rsrc1, rsrc2;
	sscanf(operands, "%*c%d %*c%d %d", &rsrc1, &rsrc2, &label);
  //printf("BNE: Source1: %d, Source2: %d, Label: %d\n", rsrc1, rsrc2, label);
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
  //printf("LA: Destination: %d, Label: %d\n", rdest, label);
  registers[rdest] = memory.data_segment[label].content;
  num_cycles += 5;
  instruction_count++;
}

void lb(string operands) {
  int32 rdest;
  int32 offset;
  sscanf(operands, "%*c%d %*c%d", &rdest, &offset);
  //printf("LB: Destination: %d, Offset: %d\n", rdest, offset);
  registers[rdest] = strings[0][registers[offset]];
  num_cycles += 6;
  instruction_count++;
}

void li(string operands) {
  int32 rdest;
  int32 imm;
  sscanf(operands, "%*c%d %d", &rdest, &imm);
  //printf("LI: Destination: %d, Imm: %d\n", rdest, imm);
  registers[rdest] = imm;
  num_cycles += 3;
  instruction_count++;
}

void subi(string operands) {
	int32 rdest, rsrc1, imm;
	sscanf(operands, "%*c%d %*c%d %d", &rdest, &rsrc1, &imm);
  //printf("SUBI: Destination: %d, Source: %d, Imm: %d\n", rdest, rsrc1, imm);
	registers[rdest] = registers[rsrc1] - imm;
  	num_cycles += 6;
  	instruction_count++;
}

void syscall() {
  int32 service_num = registers[29];
  int32 arg1 = registers[30];
  int32 arg2 = registers[31];
  switch (service_num) {
  	case 0 :
  		readString();
      break;
  	case 1 :
  		writeString();
      break;
  	case 2 :
  		run = 0;
  }
  num_cycles += 8;
  instruction_count++;
}

void readString() {
	// Empty stub for now
  //printf("readString()\n");
}

void writeString() {
	printf("%s\n", strings[registers[31]]);
}

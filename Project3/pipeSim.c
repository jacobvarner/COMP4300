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

// Setting up latches
if_id if_id_old, if_id_new;
id_ex id_ex_old, id_ex_new;
ex_mem ex_mem_old, ex_mem_new;
mem_wb mem_wb_old, mem_wb_new;

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

int32 instr_fetch(int32 currentInstructionCode, int pc);
id_ex instr_decode(int32 ir, int pc);
ex_mem instr_exe(id_ex id_ex_old);
mem_wb mem_access(ex_mem ex_mem_old);
void write_back(mem_wb mem_wb_old, int gpr);


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

    if_id_old.ir = if_id_new.ir;
    if_id_new.ir = instr_fetch(currentInstructionCode, &program_counter);

    id_ex_old = id_ex_new;
    id_ex_new = instr_decode(if_id_old.ir, &program_counter);

    ex_mem_old = ex_mem_new;
    ex_mem_new = instr_exe(id_ex_old);

    mem_wb_old = mem_wb_new;
    mem_wb_new = mem_access(ex_mem_old);

    write_back(mem_wb_old, &registers);

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

int32 instr_fetch(int currentInstructionCode, int pc) {

  // Need to check for hazards

  return currentInstructionCode;
}

id_ex instr_decode(int32 ir, int pc) {

  // Need to check for hazards

  id_ex id_ex_output;

  switch (ir) {
    case 0 : //LOAD
      load(currentText.operands);
      break;
    case 1 : //STO
      sto(currentText.operands);
      break;
    case 2 : //ADD
      int32 rd;
      int32 rs;
      int32 rt;
      sscanf(operands, "%*c%d %*c%d %d", &rd, &rs, &rt);
      id_ex_output.rd = rd;
      id_ex_output.rs = rs;
      id_ex_output.rt = rt;
      id_ex_output.op_A = registers[rs];
      id_ex_output.op_B = registers[rt];
      id_ex_output.op_code = ir;
      break;
    case 3 : //MULT
      multiply(currentText.operands);
      break;
    case 4 : //END
      run = 0;
      break;
    case 5 : //ADDI
      int32 rd;
      int32 rs;
      int32 imm;
      sscanf(operands, "%*c%d %*c%d %d", &rd, &rs, &imm);
      id_ex_output.rd = rd;
      id_ex_output.rs = rs;
      id_ex_output.op_A = registers[rs];
      id_ex_output.op_B = imm;
      id_ex_output.op_code = ir;
      break;
    case 6 : //B
      int32 label;
      sscanf(operands, "%d", &label);
      id_ex_output.rd = label;
      id_ex_output.op_code = ir;
      break;
    case 7 : //BEQZ
      int32 label, rs;
      sscanf(operands, "%*c%d %d", &rs, &label);
      id_ex_output.rs = rs;
      id_ex_output.rd = label;
      id_ex_output.op_A = registers[rs];
      id_ex_output.op_code = ir;
      break;
    case 8 : //BGE
      int32 label, rs, rt;
      sscanf(operands, "%*c%d %*c%d %d", &rs, &rt, &label);
      id_ex_output.rs = rs;
      id_ex_output.rt = rt;
      id_ex_output.rd = label;
      id_ex_output.op_A = registers[rs];
      id_ex_output.op_B = registers[rt];
      id_ex_output.op_code = ir;
      break;
    case 9 : //BNE
      int32 label, rs, rt;
      sscanf(operands, "%*c%d %*c%d %d", &rs, &rt, &label);
      id_ex_output.rs = rs;
      id_ex_output.rt = rt;
      id_ex_output.rd = label;
      id_ex_output.op_A = registers[rs];
      id_ex_output.op_B = registers[rt];
      id_ex_output.op_code = ir;
      break;
    case 10 : //LA
      int32 rd;
      int32 label;
      sscanf(operands, "%*c%d %d", &rd, &label);
      id_ex_output.rd = rd;
      id_ex_output.rs = label;
      id_ex_output.op_A = memory.data_segment[label].content;
      id_ex_output.op_code = ir;
      break;
    case 11 : //LB
      int32 rd;
      int32 offset;
      sscanf(operands, "%*c%d %*c%d", &rd, &offset);
      id_ex_output.rd = rd;
      id_ex_output.offset = offset;
      id_ex_output.op_code = ir;
      break;
    case 12 : //LI
      int32 rd;
      int32 imm;
      sscanf(operands, "%*c%d %d", &rd, &imm);
      id_ex_output.rd = rd;
      id_ex_output.op_A = imm;
      id_ex_output.op_code = ir;
      break;
    case 13 : //SUBI
      int32 rd, rs, imm;
      sscanf(operands, "%*c%d %*c%d %d", &rd, &rs, &imm);
      id_ex_output.rd = rd;
      id_ex_output.rs = rs;
      id_ex_output.op_A = registers[rs];
      id_ex_output.op_B = imm;
      id_ex_output.op_code = ir;
      break;
    case 14 : //SYSCALL
      id_ex_output.rd = 29;
      id_ex_output.rs = 30;
      id_ex_output.rt = 31;
      id_ex_output.op_A = registers[rs];
      id_ex_output.op_B = registers[rt];
      id_ex_output.op_code = ir;
      break;
    default :

  }

  return id_ex_output;
}

ex_mem instr_exe(id_ex id_ex_old) {
  switch (id_ex_old.op_code) {
    case 0 : //LOAD
      load(currentText.operands);
      break;
    case 1 : //STO
      sto(currentText.operands);
      break;
    case 2 : //ADD

      break;
    case 3 : //MULT
      multiply(currentText.operands);
      break;
    case 4 : //END
      run = 0;
      break;
    case 5 : //ADDI

      break;
    case 6 : //B

      break;
    case 7 : //BEQZ

      break;
    case 8 : //BGE

      break;
    case 9 : //BNE

      break;
    case 10 : //LA

      break;
    case 11 : //LB

      break;
    case 12 : //LI

      break;
    case 13 : //SUBI

      break;
    case 14 : //SYSCALL
      
      break;
    default :

  }
  }
}

mem_wb mem_access(ex_mem ex_mem_old){
  // TODO
}

void write_back(mem_wb mem_wb_old, int gpr) {
  // TODO
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

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
ex_mem add(int32 rd, int32 op_A, int32 op_B, int32 op_code);
void multiply(string operands);
ex_mem addi(int32 rd, int32 op_A, int32 op_B, int32 op_code);
ex_mem b(int32 rd, int32 op_code);
ex_mem beqz(in32 rd, int32 op_A, int32 op_code);
ex_mem bge(int32 rd, int32 op_A, int32 op_B, int32 op_code);
ex_mem bne(int32 rd, int32 op_A, int32 op_B, int32 op_code);
ex_mem la(int32 rd, int32 op_A, int32 op_code);
ex_mem lb(int32 rd, int32 op_A, int32 offset, int32 op_code);
ex_mem li(int32 rd, int32 op_A, int32 op_code);
ex_mem subi(int32 rd, int32 op_A, int32 op_B, int32 op_code);
ex_mem syscall();
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
    case 15 : //NOP
      // TODO
      break;
    default :

  }

  return id_ex_output;
}

ex_mem instr_exe(id_ex id_ex) {
  ex_mem output;
  switch (id_ex.op_code) {
    case 0 : //LOAD
      load(currentText.operands);
      break;
    case 1 : //STO
      sto(currentText.operands);
      break;
    case 2 : //ADD
      output = add(id_ex.rd, id_ex.op_A, id_ex.op_B, id_ex.op_code);
      break;
    case 3 : //MULT
      multiply(currentText.operands);
      break;
    case 4 : //END
      run = 0;
      break;
    case 5 : //ADDI
      output = addi(id_ex.rd, id_ex.op_A, id_ex.op_B, id_ex.op_code);
      break;
    case 6 : //B
      output = b(id_ex.rd, id_ex.op_code);
      break;
    case 7 : //BEQZ
      output = beqz(id_ex.rd, id_ex.op_A, id_ex.op_code);
      break;
    case 8 : //BGE
      output = bge(id_ex.rd, id_ex.op_A, id_ex.op_B, id_ex.op_code);
      break;
    case 9 : //BNE
      output = bne(id_ex.rd, id_ex.op_A, id_ex.op_B, id_ex.op_code);
      break;
    case 10 : //LA
      output = la(id_ex.rd, id_ex.op_A, id_ex.op_code);
      break;
    case 11 : //LB
      output = lb(id_ex.rd, id_ex.op_A, id_ex.offset, id_ex.op_code);
      break;
    case 12 : //LI
      output = li(id_ex.rd, id_ex.op_A, id_ex.op_code);
      break;
    case 13 : //SUBI
      output = subi(id_ex.rd, id_ex.op_A, id_ex.op_B, id_ex.op_code);
      break;
    case 14 : //SYSCALL
      output = syscall();
      break;
    case 15 : //NOP
      // TODO
      break;
    default :

  }

  return output;
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

ex_mem add(int32 rd, int32 op_A, int32 op_B, int32 op_code) {
  ex_mem output;
	int sum = 0;
  output.op_code = op_code;
  sum = op_A + op_B;
  output.rd = rd;
  output.alu_out = sum;
  output.op_B = sum;
  return output;
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

ex_mem addi(int32 rd, int32 op_A, int32 op_B, int32 op_code) {
  ex_mem output;
  int32 answer = op_A + op_B;
  output.op_code = op_code;
  output.rd = rd;
  output.alu_out = answer;
  output.op_B = answer;
  num_cycles += 6;
  instruction_count++;
  return output;
}

ex_mem b(int32 rd, int32 op_code) {
	ex_mem output;
  output.op_code = op_code;
	output.rd = rd;
  output.alu_out = rd;
  output.op_B = rd;
  num_cycles += 4;
  instruction_count++;
  return output;
}

ex_mem beqz(int32 rd, int32 op_A, int32 op_code) {
  ex_mem output;
	output.op_code = op_code;
	if(op_A == 0) {
		output.rd = rd;
    output.alu_out = rd;
    output.op_B = rd;
	} else {
    output.rd = 0;
    output.alu_out = 0;
    output.op_B = 0;
  }
  num_cycles += 5;
  instruction_count++;
  return output;
}

ex_mem bge(int32 rd, int32 op_A, int32 op_B, int32 op_code) {
  ex_mem output;
  output.op_code = op_code;
	if(op_A >= op_B) {
		output.rd = rd;
    output.alu_out = rd;
    output.op_B = rd;
	} else {
    output.rd = 0;
    output.alu_out = 0;
    output.op_B = 0;
  }
  num_cycles += 5;
  instruction_count++;
  return output;
}

ex_mem bne(int32 rd, int32 op_A, int32 op_B, int32 op_code) {
  ex_mem output;
  output.op_code = op_code;
	if(op_A != op_B) {
		output.rd = rd;
    output.alu_out = rd;
    output.op_B = rd;
	} else {
    output.rd = 0;
    output.alu_out = 0;
    output.op_B = 0;
  }
  num_cycles += 5;
  instruction_count++;
  return output;
}

ex_mem la(int32 rd, int32 op_A, int32 op_code) {
  ex_mem output;
  output.op_code = op_code;
  output.rd = rd;
  output.alu_out = op_A;
  output.op_B = op_A;
  num_cycles += 5;
  instruction_count++;
  return output;
}

ex_mem lb(int32 rd, int32 op_A, int32 offset, int32 op_code) {
  ex_mem output;
  output.op_code = op_code;
  output.rd = rd;
  output.op_B = op_A;
  output.alu_out = op_A;
  num_cycles += 6;
  instruction_count++;
  return output;
}

ex_mem li(int32 rd, int32 op_A, int32 op_code) {
  ex_mem output;
  output.op_code = op_code;
  output.rd = rd;
  output.op_B = op_A;
  output.alu_out = op_A;
  num_cycles += 3;
  instruction_count++;
  return output;
}

ex_mem subi(int32 rd, int32 op_A, int32 op_B, int32 op_code) {
  ex_mem output;
	output.op_code = op_code;
  output.rd = rd;
  output.op_B = op_A - op_B;
  output.alu_out = op_A - op_B;
  num_cycles += 6;
  instruction_count++;
  return output;
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

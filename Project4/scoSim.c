#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "loader.h"
#include "memory.h"
#include "read.h"

#define NUM_REGISTERS 32
#define NUM_STRINGS 3

Memory code_segment;
int program_counter;
int num_cycles;
int instruction_count;
int num_nops;
int run;
int registers[NUM_REGISTERS];
string strings[NUM_STRINGS];

// Setting up latches
if_id if_id_old, if_id_new;
id_ex id_ex_old, id_ex_new;
ex_mem ex_mem_old, ex_mem_new;
mem_wb mem_wb_old, mem_wb_new;

void syscall();

if_id instr_fetch(int32 currentInstructionCode, string operands, int *pc);
id_ex instr_decode(if_id if_id_old, int *pc);
ex_mem instr_exe(id_ex id_ex_old, ex_mem ex_mem_old, mem_wb mem_wb_new);
mem_wb mem_access(ex_mem ex_mem_old);
void write_back(mem_wb mem_wb_old);



int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("You must provide an assembly file to be read.\n");
    return 1; /* Error: Must provide assembly file to be read. */
  }

  char[] program_name = argv[1];

	printf("Loading into memory...\n\n");
	code_segment = load_program(program_name);
	printf("\nLoading complete!\n\n");

  pc = initiate_simulator();

  simulator_run(code_segment, pc);

  return 0;
}

	run = 1;
	Text currentText;
	int currentInstructionCode;

	while (run) {
    currentText = loadText(program_counter + TEXT_SEGMENT_BASE_ADDRESS, memory);
		//printf("%s \n", currentText.instruction);
		currentInstructionCode = currentText.instruction_code;

    if_id_old = if_id_new;
    if_id_new = instr_fetch(currentInstructionCode, currentText.operands, &program_counter);

    id_ex_old = id_ex_new;
    printf("if_id_old.ir: %d\n", if_id_old.ir);
    id_ex_new = instr_decode(if_id_old, &program_counter);
    printf("Before instr_exe; id_ex_new.op_A: %d\n", id_ex_new.op_A);

    ex_mem_old = ex_mem_new;
    printf("Before instr_exe; id_ex_old.op_A: %d \n\n", id_ex_old.op_A);
    ex_mem_new = instr_exe(id_ex_old, ex_mem_old, mem_wb_new);

    mem_wb_old = mem_wb_new;
    mem_wb_new = mem_access(ex_mem_old);

    write_back(mem_wb_old);

    program_counter++;
	}

  double speedup = (8 * (double)instruction_count) / (double)num_cycles;

  // Output summary
	printf("\nRESULTS:\n\nRegister[0]: %d\nRegister[1]: %d\nRegister[2]: %d\n\
Register[3]: %d\nRegister[29]: %d\nRegister[30]: %d\n\
Register[31]: %d\n\nC = %d\nIC = %d\n[8*IC]/C = %f\n\nNOPS: %d\n\n", registers[0],
  registers[1], registers[2], registers[3], registers[29], registers[30],
  registers[31], num_cycles, instruction_count, speedup, num_nops);

	return 0;
}

if_id instr_fetch(int currentInstructionCode, string operands, int *pc) {
  if_id output;
  output.ir = currentInstructionCode;
  strcpy(output.operands, operands);
  pc++;
  return output;
}

id_ex instr_decode(if_id if_id, int *pc) {
  id_ex output;
  int32 rd, rs, rt, offset, label, imm;
  output.op_code = if_id.ir;

  switch (if_id.ir) {
    case 0 : //LOAD
      // left over
      break;
    case 1 : //STO
      // left over
      break;
    case 2 : //ADD
      printf("ADD decodec \n");
      sscanf(if_id.operands, "%*c%d %*c%d %d", &rd, &rs, &rt);
      printf("ADD: rd: %d, rs: %d, rt: %d\n", rd, rs, rt);
      output.rd = rd;
      output.rs = rs;
      output.rt = rt;
      output.op_A = registers[rs];
      output.op_B = registers[rt];
      break;
    case 3 : //MULT
      // left over
      break;
    case 4 : //END
      run = 0;
      break;
    case 5 : //ADDI
      printf("ADDI decoded \n");
      sscanf(if_id.operands, "%*c%d %*c%d %d", &rd, &rs, &imm);
      printf("ADDI: rd: %d, rs: %d, imm: %d\n", rd, rs, imm);
      output.rd = rd;
      output.rs = rs;
      output.op_A = registers[rs];
      output.op_B = imm;
      num_cycles += 6;
      instruction_count++;
      break;
    case 6 : //B
      printf("B decoded \n");
      sscanf(if_id.operands, "%d", &label);
      printf("B: label: %d", label);
      pc += label;
      output.op_code = 15;
      num_cycles += 4;
      instruction_count++;
      break;
    case 7 : //BEQZ
      printf("BEQZ decoded \n");
      sscanf(if_id.operands, "%*c%d %d", &rs, &label);
      printf("BEQZ: rs: %d, label: %d \n", rs, label);
      output.rs = rs;
      output.op_A = registers[rs];
      if (output.op_A == 0) {
        pc += label;
      }
      output.op_code = 15;
      num_cycles += 5;
      instruction_count++;
      break;
    case 8 : //BGE
      printf("BGE decoded \n");
      sscanf(if_id.operands, "%*c%d %*c%d %d", &rs, &rt, &label);
      printf("BGE: rs: %d, rt: %d, label: %d\n", rs, rt, label);
      output.rs = rs;
      output.rt = rt;
      output.op_A = registers[rs];
      output.op_B = registers[rt];
      if (output.op_A >= output.op_B) {
        pc += label;
      }
      output.op_code = 15;
      num_cycles += 5;
      instruction_count++;
      break;
    case 9 : //BNE
      printf("BNE decoded \n");
      sscanf(if_id.operands, "%*c%d %*c%d %d", &rs, &rt, &label);
      printf("BNE: rs: %d, rt: %d, label: %d\n", rs, rt, label);
      output.rs = rs;
      output.rt = rt;
      output.op_A = registers[rs];
      output.op_B = registers[rt];
      if (output.op_A != output.op_B) {
        pc += label;
      }
      output.op_code = 15;
      num_cycles += 5;
      instruction_count++;
      break;
    case 10 : //LA
      printf("LA decoded \n");
      sscanf(if_id.operands, "%*c%d %d", &rd, &label);
      printf("LA: rd: %d, label: %d\n", rd, label);
      output.rd = rd;
      output.rs = label;
      output.op_A = memory.data_segment[label].content;
      printf("output.op_A: %d\n", output.op_A);
      num_cycles += 5;
      instruction_count++;
      break;
    case 11 : //LB
      printf("LB decoded \n");
      sscanf(if_id.operands, "%*c%d %*c%d", &rd, &offset);
      printf("LB: rd: %d, offset: %d\n", rd, offset);
      output.rd = rd;
      output.offset = offset;
      num_cycles += 6;
      instruction_count++;
      break;
    case 12 : //LI
      printf("LI decoded \n");
      sscanf(if_id.operands, "%*c%d %d", &rd, &imm);
      printf("LI: Destination: %d, Imm: %d\n", rd, imm);
      output.rd = rd;
      output.op_A = imm;
      num_cycles += 3;
      instruction_count++;
      break;
    case 13 : //SUBI
      printf("SUBI decoded \n");
      sscanf(if_id.operands, "%*c%d %*c%d %d", &rd, &rs, &imm);
      printf("SUBI: Destination: %d, Source: %d, Imm: %d\n", rd, rs, imm);
      output.rd = rd;
      output.rs = rs;
      output.op_A = registers[rs];
      output.op_B = imm;
      num_cycles += 6;
      instruction_count++;
      break;
    case 14 : //SYSCALL
      printf("SYSCALL decoded \n");
      num_cycles += 8;
      instruction_count++;
      break;
    case 15 : //NOP
      printf("NOP decoded \n");
      num_nops++;
      break;
  }

  printf("output.op_A: %d\n", output.op_A);
  return output;
}

ex_mem instr_exe(id_ex id_ex, ex_mem ex_mem_var, mem_wb mem_wb) {
  ex_mem output;

  // Forwarding
  if (id_ex.rs == ex_mem_var.rd) {
    id_ex.op_A = ex_mem_var.alu_out;
    printf("FORWARDING: %d\n", id_ex.op_A);
  }

  if (id_ex.rt == ex_mem_var.rd) {
    id_ex.op_B = ex_mem_var.alu_out;
  }

  // Mem hazards
  if (mem_wb.rd == id_ex.rs) {
    id_ex.op_A = mem_wb.alu_out;
    printf("MEM HAZARD: %d\n", id_ex.op_A);
  }
  if (mem_wb.rd == id_ex.rt) {
    id_ex.op_B = mem_wb.alu_out;
  }

  switch (id_ex.op_code) {
    case 0 : //LOAD
      // left over
      break;
    case 1 : //STO
      // left over
      break;
    case 2 : //ADD
      printf("ADD executed\n");
      output.alu_out = id_ex.op_A + id_ex.op_B;
      break;
    case 3 : //MULT
      // left over
      break;
    case 4 : //END
      // left over
      break;
    case 5 : //ADDI
      printf("ADDI executed \n");
      output.alu_out = id_ex.op_A + id_ex.op_B;
      break;
    case 6 : //B
      printf("B executed \n");
      // already done
      break;
    case 7 : //BEQZ
      printf("BEQZ executed \n");
      // already done
      break;
    case 8 : //BGE
      printf("BGE executed \n");
      // already done
      break;
    case 9 : //BNE
      printf("BNE executed \n");
      // already done
      break;
    case 10 : //LA
      printf("LA executed \n");
      printf("id_ex.op_A: %d\n", id_ex.op_A);
      output.alu_out = id_ex.op_A;
      break;
    case 11 : //LB
      printf("LB executed \n");
      output.alu_out = id_ex.op_A;
      break;
    case 12 : //LI
      printf("LI executed \n");
      printf("OP A: %d \n", id_ex.op_A);
      output.alu_out = id_ex.op_A;
      break;
    case 13 : //SUBI
      printf("SUBI executed \n");
      output.alu_out = id_ex.op_A - id_ex.op_B;
      break;
    case 14 : //SYSCALL
      printf("SYSCALL executed \n");
      syscall();
      break;
    case 15 : //NOP
      printf("NOP executed \n");
      break;
  }

  output.op_code = id_ex.op_code;
  output.op_B = id_ex.op_B;
  output.rd = id_ex.rd;

  return output;
}

pc initiate_simulator() {
  pc pc;
  pc.program_counter = 0;
  pc.num_cycles = 0;
  pc.instruction_count = 0;
  pc.num_nops = 0;

  return pc;
}

mem_wb mem_access(ex_mem ex_mem_old){
  mem_wb output;
  if (ex_mem_old.op_code == 11) {
    output.mdr = memory.data_segment[ex_mem_old.alu_out].content;
  } else if (ex_mem_old.op_code == 12) {
    output.mdr = memory.data_segment[ex_mem_old.alu_out].content;
  } else if (ex_mem_old.op_code == 13) {
    output.mdr = ex_mem_old.alu_out;
  }

  output.op_code = ex_mem_old.op_code;
  output.op_B = ex_mem_old.op_B;
  output.alu_out = ex_mem_old.alu_out;
  output.rd = ex_mem_old.rd;

  return output;
}

void write_back(mem_wb mem_wb) {
  if (ex_mem_old.op_code == 11) {
    registers[mem_wb.op_B] = mem_wb.mdr;
  } else if (ex_mem_old.op_code == 12) {
    registers[mem_wb.op_B] = mem_wb.mdr;
  } else if (ex_mem_old.op_code == 13) {
    registers[mem_wb.op_B] = mem_wb.mdr;
  } else {
    registers[mem_wb.rd] = mem_wb.alu_out;
  }
}

void syscall() {
  printf("%s\n", registers[31]);

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
}

void readString() {
	// Empty stub for now
  //printf("readString()\n");
}

void writeString() {
	printf("%s\n", strings[registers[31]]);
}

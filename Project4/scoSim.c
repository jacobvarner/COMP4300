#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "loader.h"
#include "memory.h"
#include "read.h"

#define NUM_REGISTERS 32
#define NUM_FP_REGISTERS 16
#define NUM_FUS 5

typedef int bool;
#define true 1
#define false 0

// Setting up latches
if_id if_id_old, if_id_new;
id_ex id_ex_old, id_ex_new;
ex_mem ex_mem_old, ex_mem_new;
mem_wb mem_wb_old, mem_wb_new;

void syscall();
if_id instr_fetch(int currentInstructionCode, string operands, int *pc);
id_ex instr_decode(if_id if_id_old, int *pc);
ex_mem instr_exe(id_ex id_ex_old, ex_mem ex_mem_old, mem_wb mem_wb_new);
mem_wb mem_access(ex_mem ex_mem_old);
void write_back(mem_wb mem_wb_old);
pc initiate_simulator();
scoreboard initiate_scoreboard(Memory code_segment);
void simulator_run(Memory code_segment, pc *pc);
void print_scoreboard(scoreboard scoreboard, Memory code_segment);
bool issue_instruction(scoreboard *scoreboard, Memory code_segment, pc *pc, Memory *fetch_buffer);
op decode(Text instruction);
bool check_fu_busy(scoreboard scoreboard, op op);
bool check_waw(scoreboard scoreboard, op op);
void set_fetch_buffer(Memory fetch_buffer, Text instruction);
void read_operands(scoreboard scoreboard, int int_register_file[], float fp_register_file[], Memory fetch_buffer);
char zero(int input);



int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("You must provide an assembly file to be read.\n");
    return 1; /* Error: Must provide assembly file to be read. */
  }

	printf("Loading into memory...\n\n");
	Memory code_segment = load_program(argv[1]);
	printf("\nLoading complete!\n\n");

  pc pc = initiate_simulator();

  simulator_run(code_segment, &pc);

  return 0;
}

void simulator_run(Memory code_segment, pc *pc) {
  int int_register_file[NUM_REGISTERS];
  float fp_register_file[NUM_FP_REGISTERS];
  Memory fetch_buffer;
  fetch_buffer.num_instructions = 0;

  scoreboard scoreboard = initiate_scoreboard(code_segment);

  int running = 1;

  while (running) {
    // Prints the scoreboard in its current state to help debug
    printf("PC: %d\n", pc->program_counter);
    print_scoreboard(scoreboard, code_segment);
    bool stall = issue_instruction(&scoreboard, code_segment, pc, &fetch_buffer);
    read_operands(scoreboard, int_register_file, fp_register_file, fetch_buffer);
    printf("PC: %d\n", pc->program_counter);
    print_scoreboard(scoreboard, code_segment);

    running = 0;
  }
}

pc initiate_simulator() {
  pc pc;
  pc.program_counter = 0;
  pc.num_cycles = 0;
  pc.instruction_count = 0;
  pc.num_nops = 0;

  return pc;
}

scoreboard initiate_scoreboard(Memory code_segment) {
  scoreboard scoreboard;
  printf("# Instructions: %d\n", code_segment.num_instructions);
  i_status i_status[code_segment.num_instructions];
  for (int i = 0; i < code_segment.num_instructions; i++) {
    strcpy(i_status[i].instruction, code_segment.text_segment[i].instruction);
    strcat(i_status[i].instruction, " ");
    strcat(i_status[i].instruction, code_segment.text_segment[i].operands);
    i_status[i].issue = 0;
    i_status[i].read = 0;
    i_status[i].exe = 0;
    i_status[i].write = 0;
  }

  fu_status fu_status[NUM_FUS];
  strcpy(fu_status[0].name, "Branch");
  fu_status[0].busy = 0;
  strcpy(fu_status[1].name, "Multiply");
  fu_status[1].busy = 0;
  strcpy(fu_status[2].name, "Subtract");
  fu_status[2].busy = 0;
  strcpy(fu_status[3].name, "Add");
  fu_status[3].busy = 0;
  strcpy(fu_status[4].name, "Load/Store");
  fu_status[4].busy = 0;

  int r_status[NUM_REGISTERS];
  int fpr_status[NUM_FP_REGISTERS];

  for (int i = 0; i < NUM_REGISTERS; i++) {
    r_status[i] = -1;
  }

  for (int i = 0; i < NUM_FP_REGISTERS; i++) {
    fpr_status[i] = -1;
  }

  memcpy(scoreboard.i_status, i_status, sizeof(i_status));
  memcpy(scoreboard.fu_status, fu_status, sizeof(fu_status));
  memcpy(scoreboard.r_status, r_status, sizeof(r_status));
  memcpy(scoreboard.fpr_status, fpr_status, sizeof(fpr_status));

  return scoreboard;
}

void print_scoreboard(scoreboard scoreboard, Memory code_segment) {
  printf("----------------------------------------------------------------\n");
  printf("                         SCOREBOARD\n");
  printf("----------------------------------------------------------------\n\n");
  printf("Instruction      Issue   Read   Exe.   Write\n");
  printf("----------------------------------------------------------------\n");
  for (int i = 0; i < code_segment.num_instructions; i++) {
    printf("%-15s  %-6c  %-5c  %-5c  %-5c\n", scoreboard.i_status[i].instruction, zero(scoreboard.i_status[i].issue),
      zero(scoreboard.i_status[i].read), zero(scoreboard.i_status[i].exe), zero(scoreboard.i_status[i].write));
  }
  printf("\nName        Busy   Op   Fi   Fj   Fk   Qj   Qk   Rj   Rk\n");
  printf("----------------------------------------------------------------\n");
  for (int i = 0; i < NUM_FUS; i++) {
    printf("%-10s  %-4d   %-2c   %-2c   %-2c   %-2c   %-2c   %-2c   %-2c   %-2c\n", scoreboard.fu_status[i].name,
      scoreboard.fu_status[i].busy, zero(scoreboard.fu_status[i].op), zero(scoreboard.fu_status[i].fi),
      zero(scoreboard.fu_status[i].fj), zero(scoreboard.fu_status[i].fk), zero(scoreboard.fu_status[i].qj),
      zero(scoreboard.fu_status[i].qk), zero(scoreboard.fu_status[i].rj), zero(scoreboard.fu_status[i].rk));
  }
  printf("\n");
  printf("Registers     0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15\n");
  printf("----------------------------------------------------------------\n");
  printf("              ");
  for (int i = 0; i < 16; i++) {
    if (scoreboard.r_status[i] != -1) {
      printf("%-2d ", scoreboard.r_status[i]);
    } else {
      printf("   ");
    }
  }
  printf("\n\n");
  printf("Registers     16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31\n");
  printf("----------------------------------------------------------------\n");
  printf("              ");
  for (int i = 16; i < 32; i++) {
    if (scoreboard.r_status[i] != -1) {
      printf("%-2d ", scoreboard.r_status[i]);
    } else {
      printf("   ");
    }
  }
  printf("\n\n");
  printf("FP Registers  0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15\n");
  printf("----------------------------------------------------------------\n");
  printf("              ");
  for (int i = 0; i < 16; i++) {
    if (scoreboard.fpr_status[i] != -1) {
      printf("%-2d ", scoreboard.fpr_status[i]);
    } else {
      printf("   ");
    }
  }
  printf("\n\n");
}

bool issue_instruction(scoreboard *scoreboard, Memory code_segment, pc *pc, Memory *fetch_buffer) {
  Text instruction = code_segment.text_segment[pc->program_counter];
  // gets the operands from the instruction
  op op = decode(instruction);
  if (check_fu_busy(*scoreboard, op) == true) {
    // Stall
    printf("STALL\n");
    return true;
  }
  if (check_waw(*scoreboard, op) == true) {
    // Stall
    printf("STALL\n");
    return true;
  }
  set_fetch_buffer(*fetch_buffer, instruction);
  // Updates the FU Status portion of the scoreboard
  switch (op.op_type) {
    case 0: // Branch
      scoreboard->fu_status[0].busy = 1;
      scoreboard->fu_status[0].op = op.op_code;
      if (op.reg_dest != -1) {
        scoreboard->fu_status[0].fi = op.reg_dest;
      }
      if (op.reg_a != -1) {
        scoreboard->fu_status[0].fj = op.reg_a;
      }
      if (op.reg_b != -1) {
        scoreboard->fu_status[0].fk = op.reg_b;
      }
      if (op.reg_a != -1) {
        scoreboard->fu_status[0].rj = 1;
      }
      if (op.reg_b != -1) {
        scoreboard->fu_status[0].rk = 1;
      }
      break;
    case 1: // Multiply
      scoreboard->fu_status[1].busy = 1;
      scoreboard->fu_status[1].op = op.op_code;
      if (op.reg_dest != -1) {
        scoreboard->fu_status[1].fi = op.reg_dest;
      }
      if (op.reg_a != -1) {
        scoreboard->fu_status[1].fj = op.reg_a;
      }
      if (op.reg_b != -1) {
        scoreboard->fu_status[1].fk = op.reg_b;
      }
      if (op.reg_a != -1) {
        scoreboard->fu_status[1].rj = 1;
      }
      if (op.reg_b != -1) {
        scoreboard->fu_status[1].rk = 1;
      }
      break;
    case 2: // Add
      scoreboard->fu_status[2].busy = 1;
      scoreboard->fu_status[2].op = op.op_code;
      if (op.reg_dest != -1) {
        scoreboard->fu_status[2].fi = op.reg_dest;
      }
      if (op.reg_a != -1) {
        scoreboard->fu_status[2].fj = op.reg_a;
      }
      if (op.reg_b != -1) {
        scoreboard->fu_status[2].fk = op.reg_b;
      }
      if (op.reg_a != -1) {
        scoreboard->fu_status[2].rj = 1;
      }
      if (op.reg_b != -1) {
        scoreboard->fu_status[2].rk = 1;
      }
      break;
    case 3: // Subtract
      scoreboard->fu_status[3].busy = 1;
      scoreboard->fu_status[3].op = op.op_code;
      if (op.reg_dest != -1) {
        scoreboard->fu_status[3].fi = op.reg_dest;
      }
      if (op.reg_a != -1) {
        scoreboard->fu_status[3].fj = op.reg_a;
      }
      if (op.reg_b != -1) {
        scoreboard->fu_status[3].fk = op.reg_b;
      }
      if (op.reg_a != -1) {
        scoreboard->fu_status[3].rj = 1;
      }
      if (op.reg_b != -1) {
        scoreboard->fu_status[3].rk = 1;
      }
      break;
    case 4: // Load/Store
      scoreboard->fu_status[4].busy = 1;
      scoreboard->fu_status[4].op = op.op_code;
      if (op.reg_dest != -1) {
        scoreboard->fu_status[4].fi = op.reg_dest;
      }
      if (op.reg_a != -1) {
        scoreboard->fu_status[4].fj = op.reg_a;
      }
      if (op.reg_b != -1) {
        scoreboard->fu_status[4].fk = op.reg_b;
      }
      if (op.reg_a != -1) {
        scoreboard->fu_status[4].rj = 1;
      }
      if (op.reg_b != -1) {
        scoreboard->fu_status[4].rk = 1;
      }
      break;
  }
  // Updates the register status in the scoreboard
  if (op.op_code <= 11) {
    if (op.reg_dest != -1) {
      scoreboard->r_status[op.reg_dest] = op.op_type;
    }
  } else {
    if (op.reg_dest != -1) {
      scoreboard->fpr_status[op.reg_dest] = op.op_type;
    }
  }
  // Updates the instruction status in the scoreboard
  scoreboard->i_status[pc->program_counter].issue = pc->num_cycles + 1;
  pc->program_counter++;
  return false;
}

op decode(Text instruction) {
  op op;
  op.op_code = instruction.instruction_code;
  op.op_type = instruction.instruction_type;
  switch (instruction.instruction_code) {
    case 0: // nop
      op.reg_a = -1;
      op.reg_b = -1;
      op.reg_dest = -1;
      break;
    case 1: // add
      sscanf(instruction.operands, "%*c%d %*c%d %d", &op.reg_dest, &op.reg_a, &op.reg_b);
      break;
    case 2: // addi
      sscanf(instruction.operands, "%*c%d %*c%d", &op.reg_dest, &op.reg_a);
      op.reg_b = -1;
      break;
    case 3: // b
      op.reg_a = -1;
      op.reg_b = -1;
      op.reg_dest = -1;
      break;
    case 4: // beqz
      sscanf(instruction.operands, "%*c%d", &op.reg_a);
      op.reg_b = -1;
      op.reg_dest = -1;
      break;
    case 5: // bge
      sscanf(instruction.operands, "%*c%d %*c%d", &op.reg_a, &op.reg_b);
      op.reg_dest = -1;
      break;
    case 6: // bne
      sscanf(instruction.operands, "%*c%d %*c%d", &op.reg_a, &op.reg_b);
      op.reg_dest = -1;
      break;
    case 7: // la
      sscanf(instruction.operands, "%*c%d", &op.reg_dest);
      op.reg_a = -1;
      op.reg_b = -1;
      break;
    case 8: // lb
      sscanf(instruction.operands, "%*c%d %*c%d", &op.reg_dest, &op.reg_a);
      op.reg_b = -1;
      break;
    case 9: // li
      sscanf(instruction.operands, "%*c%d", &op.reg_dest);
      op.reg_a = -1;
      op.reg_b = -1;
      break;
    case 10: // subi
      sscanf(instruction.operands, "%*c%d %*c%d", &op.reg_dest, &op.reg_a);
      op.reg_b = -1;
      break;
    case 11: // syscall
      op.reg_a = -1;
      op.reg_b = -1;
      op.reg_dest = -1;
      break;
    case 12: // fadd
      sscanf(instruction.operands, "%*c%d %*c%d %d", &op.reg_dest, &op.reg_a, &op.reg_b);
      break;
    case 13: // fmul
      sscanf(instruction.operands, "%*c%d %*c%d %d", &op.reg_dest, &op.reg_a, &op.reg_b);
      break;
    case 14: // fsub
      sscanf(instruction.operands, "%*c%d %*c%d %d", &op.reg_dest, &op.reg_a, &op.reg_b);
      break;
    case 15: // l.d
      sscanf(instruction.operands, "%*c%d %*c%d", &op.reg_dest, &op.reg_a);
      op.reg_b = -1;
      break;
    case 16: // s.d
      sscanf(instruction.operands, "%*c%d %*c%d", &op.reg_a, &op.reg_dest);
      op.reg_b = -1;
      break;
  }

  return op;
}

bool check_fu_busy(scoreboard scoreboard, op op) {
  if (op.op_type == 0) {
    // Branch FU
    if (scoreboard.fu_status[0].busy == 1) {
      return true;
    } else {
      return false;
    }
  } else if (op.op_type == 1) {
    // Multiply FU
    if (scoreboard.fu_status[1].busy == 1) {
      return true;
    } else {
      return false;
    }
  } else if (op.op_type == 2) {
    // Add FU
    if (scoreboard.fu_status[2].busy == 1) {
      return true;
    } else {
      return false;
    }
  } else if (op.op_type == 3) {
    // Subtract FU
    if (scoreboard.fu_status[3].busy == 1) {
      return true;
    } else {
      return false;
    }
  } else if (op.op_type == 4) {
    // Load/Store FU
    if (scoreboard.fu_status[4].busy == 1) {
      return true;
    } else {
      return false;
    }
  } else {
    return false;
  }
}

bool check_waw(scoreboard scoreboard, op op) {
  if (op.op_code <= 11) {
    // Integer registers
    if (op.reg_dest == -1) {
      // No destination register
      return false;
    }
    if (scoreboard.r_status[op.reg_dest] != -1) {
      // destination register is in use
      return true;
    } else {
      return false;
    }
  } else {
    // FP registers
    if (op.reg_dest == -1) {
      // No destination register
      return false;
    }
    if (scoreboard.fpr_status[op.reg_dest] != -1) {
      // destination registers is in user
      return true;
    } else {
      return false;
    }
  }
}

void set_fetch_buffer(Memory fetch_buffer, Text instruction) {
  Text temp;
  // Copies the instruction to a temporary variable
  temp.instruction_code = instruction.instruction_code;
  strcpy(temp.instruction, instruction.instruction);
  strcpy(temp.operands, instruction.operands);
  // Stores the instruction in the buffer at the first available index
  fetch_buffer.text_segment[fetch_buffer.num_instructions] = temp;
  fetch_buffer.num_instructions++;
}

void read_operands(scoreboard scoreboard, int int_register_file[], float fp_register_file[], Memory fetch_buffer){
  // TODO
}

char zero(int input) {
  if (input == 0) {
    return ' ';
  } else {
    char c = input + '0';
    return c;
  }
}

/* mem_wb mem_access(ex_mem ex_mem_old){
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

  int service_num = registers[29];
  int arg1 = registers[30];
  int arg2 = registers[31];

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
} */

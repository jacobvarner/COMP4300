#include <stdio.h>
#include <stdlib.h>

typedef int int32;
typedef int mem_addr;

/* These are just set at arbitrary values for now */
#define TEXT_SEG_SIZE 32
#define DATA_SEG_SIZE 32
#define STACK_SIZE 32
#define TEXT_MIN 0
#define DATA_MIN 32
#define STACK_MIN 64

mem_addr text_base = TEXT_MIN;
mem_addr data_base = DATA_MIN;
mem_addr stack_base = STACK_MIN;
int32 text_seg[TEXT_SEG_SIZE];
int32 data_seg[DATA_SEG_SIZE];
int32 stack[STACK_SIZE];

void write_addr(char[] content, mem_addr location);

/*
* Reads in an assembly file and initializes the simulator.
*/
int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("You must provide an assembly file to be read.");
    return 1; /* Error: Must provide assembly file to be read. */
  }

  char line[256];
  FILE *file_ptr;
  if ((file_ptr = fopen(argv[1], "r")) == NULL) {
    printf("Error! File could not be opened.");
    return 1; /* Error: File could not be read. */
  }

  mem_addr mem_index = 0;
  while (fgets(line, sizeof(line), file_ptr)) {
    /* Writes each line into a location in memory. */
    mem_addr location = mem_index;
    write_addr(line, location);
    mem_index++;
  }

}

/*
* Takes an address in memory as an input and returns the contents at that address.
*/
int32* read_addr(mem_addr location) {
  int offset;
  if (location >= text_base && location < data_base) {
    offset = text_base;
    if (offset >= TEXT_SEG_SIZE) {
      exit(0); /* Error: outside of the designated memory. */
    }
    return &text_seg[offset];
  } else if (location >= data_base && location < stack_base) { /* Needs to be some top cap. */
    offset = data_base;
    if (offset >= DATA_SEG_SIZE) {
      exit(0); /* Error: outside of the designated memory. */
    }
    return &data_seg[offset];
  } else {
    offset = stack_base;
    if (offset >= STACK_SIZE) {
      exit(0); /* Error: outside of the designated memory. */
    }
    return &stack[offset];
  }
}

/*
* Places the input contents into the input address in memory.
*/
void write_addr(char[] content, mem_addr location) {
  int offset;
  if (location >= text_base && location < data_base) {
    offset = text_base;
    if (offset >= TEXT_SEG_SIZE) {
      exit(0); /* Error: outside of the designated memory. */
    }
    text_seg[offset] = content;
  } else if (location >= data_base && location < stack_base) { /* Needs to be some top cap. */
    offset = data_base;
    if (offset >= DATA_SEG_SIZE) {
      exit(0); /* Error: outside of the designated memory. */
    }
    data_seg[offset] = content;
  } else {
    offset = stack_base;
    if (offset >= STACK_SIZE) {
      exit(0); /* Error: outside of the designated memory. */
    }
    stack[offset] = content;
  }
}

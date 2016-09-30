#ifndef LOADER_H_
#define LOADER_H_

#include "memory.h"

Memory loadProgramIntoMemory(char program_file_path[]);

/*
 * Set the current segment that is be loaded into memory
 */
void setCurrentSegment(char line[]);

int assignInstructionCode(string instruction);

#endif // LOADER_H_

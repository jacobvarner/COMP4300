#ifndef READ_H_
#define READ_H_

#include "memory.h"

/*
 * Reads an address and returns the contents of the address from the text segment in memory
 */
Text loadText(int32 address, Memory memory);

/*
 * Reads an address and returns the contents of the address from the data segment in memory
 */
Data loadData(int32 address, Memory memory);


#endif // READ_H_

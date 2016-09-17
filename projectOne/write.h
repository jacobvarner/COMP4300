#ifndef WRITE_H_
#define WRITE_H_

#include "memory.h"

/*
 * Writes text to the text segment of memory
 */
void writeToTextSegment(Text text_segment[], Text text, int32 next_address);

/*
 * Writes data to the data segment of memory
 */
void writeToDataSegment(Data data_segment[], Data data, int32 next_address);


#endif // WRITE_H_

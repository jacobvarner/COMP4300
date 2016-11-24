#include "read.h"
#include "memory.h"

Text loadText(int address, Memory memory) {
	int index = address - TEXT_SEGMENT_BASE_ADDRESS;
	return memory.text_segment[index];
}

Data loadData(int address, Memory memory) {
	int index = address - DATA_SEGMENT_BASE_ADDRESS;
	return memory.data_segment[index];
}

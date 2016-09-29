#include "read.h"
#include "memory.h"

Text loadText(int32 address, Memory memory) {
	int32 index = address - TEXT_SEGMENT_BASE_ADDRESS;
	return memory.text_segment[index];
}

Data loadData(int32 address, Memory memory) {
	int32 index = address - DATA_SEGMENT_BASE_ADDRESS;
	return memory.data_segment[index];
}

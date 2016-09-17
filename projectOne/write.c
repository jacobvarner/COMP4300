#include "write.h"

void writeToTextSegment(Text text_segment[], Text text, int32 next_address) {
	int32 index = next_address - TEXT_SEGMENT_BASE_ADDRESS;
	text_segment[index] = text;
}

void writeToDataSegment(Data data_segment[], Data data, int32 next_address) {
	int32 index = next_address - DATA_SEGMENT_BASE_ADDRESS;
	data_segment[index] = data;
}



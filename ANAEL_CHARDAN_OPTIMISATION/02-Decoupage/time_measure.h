#ifndef _time_measure_h
#define _time_measure_h

#include "../common/bmp.h"

#define LOOPS 300

struct __process_data {
	int (* data_processing_function)(struct __process_data * process_data);
	uint32_t x0, y0, x1, y1;
	bmp_header_t source_header;
	uint8_t * source_bitmap;
	bmp_header_t destination_header;
	uint8_t * destination_bitmap;
	char * output_file_path;
};

typedef struct __process_data process_data_t;

int init_process_data(process_data_t * process_data, int argc, const char * argv[]);
int dispose_process_data(process_data_t * process_data);

#endif /* !_time_measure */
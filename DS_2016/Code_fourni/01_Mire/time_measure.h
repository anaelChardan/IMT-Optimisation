#ifndef _time_measure_h
#define _time_measure_h

#define LOOPS 40
#define WIDTH 15//*10*5
#define HEIGHT 12//*10*5

struct __process_data {
	int (* data_processing_function)(struct __process_data * process_data);
	uint8_t * bitmap;
	const char * output_file_path;
};

typedef struct __process_data process_data_t;

int init_process_data(process_data_t * process_data, int argc, const char * argv[]);
int dispose_process_data(process_data_t * process_data);

#endif /* !_time_measure */
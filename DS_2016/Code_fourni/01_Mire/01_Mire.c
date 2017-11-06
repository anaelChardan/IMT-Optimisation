#include <stdio.h>
#include <stdlib.h> // malloc
#include <string.h> // memset
#include <stdint.h> // uintxx_t types

#include "time_measure.h"
#include "../../../common/image_utils.h"

int basic_data_processing(process_data_t * process_data);

int init_process_data(process_data_t * process_data, int argc, const char * argv[]) {
	int status = 0;
	
	if( process_data != NULL ) {
		memset(process_data, 0, sizeof(process_data_t));
		// affect processing function
		process_data->data_processing_function = basic_data_processing;
		switch( argc ) {
			case 2:
				// allocate and initialize bitmap
				process_data->bitmap = (uint8_t *) malloc(WIDTH * HEIGHT * sizeof(uint8_t));
				if( process_data->bitmap ) {
					process_data->output_file_path = argv[1];
				} else status = -2; // bitmap allocation issue
				break;
			default:
				fprintf(stderr, "usage : %s <output BMP file path>\n", argv[0]);
				status = -3;
				break;
		}
	} else status = -1; // parameter issue
	
	return status;
}


int basic_data_processing(process_data_t * process_data) {
	int status = 0;
	uint32_t x = 0, y = 0;
	
	if( (process_data != NULL) && (process_data->bitmap != NULL) ) {
		for( y = 0; y < HEIGHT; y++) {
			for(x = 0; x < WIDTH; x++) {
				if( x <= (WIDTH / 3) ) {
					// first third : value = y + x
					process_data->bitmap[y * WIDTH + x] = y + x;
				}
				if( x > (WIDTH / 3) ) {
					process_data->bitmap[y * WIDTH + x] = (((x - (WIDTH / 3) + y) % 2) == 0) ? 0 : 255;
				}
			}
		}
	} else status = -1; // parameter issue
	
	return status;
}


int dispose_process_data(process_data_t * process_data) {
	int status = 0;
	bmp_header_t header;
	
	if( (process_data != NULL) ) {
		if( process_data->bitmap != NULL ) {
			if( process_data->output_file_path ) {
				// save bitmap in BMP format
				init_bmp_header(&header);
				header.width = WIDTH;
				header.height = HEIGHT;
				save_bitmap(process_data->output_file_path, &header, process_data->bitmap);
			}
			free(process_data->bitmap);
			process_data->bitmap = NULL;
		}
	} else status = -1; // parameter issue
	
	return status;
}


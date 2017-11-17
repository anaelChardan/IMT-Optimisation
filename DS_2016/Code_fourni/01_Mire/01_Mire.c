#include <stdio.h>
#include <stdlib.h> // malloc
#include <string.h> // memset
#include <stdint.h> // uintxx_t types

#include "time_measure.h"
#include "../../../common/image_utils.h"

int basic_data_processing(process_data_t * process_data);
int pointer_data_processing(process_data_t * process_data);

int init_process_data(process_data_t * process_data, int argc, const char * argv[]) {
	int status = 0;
	
	if( process_data != NULL ) {
		memset(process_data, 0, sizeof(process_data_t));
		// affect processing function
//		process_data->data_processing_function = basic_data_processing;
		process_data->data_processing_function = pointer_data_processing;
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

int faster_data_processing(process_data_t * process_data) {
    int status = 0;
    uint8_t * p = NULL, value = 0;
    uint32_t line_counter = 0, gradient_start_value = 0, column_counter = 0, checkerboard_start_value = 0xFF;

    if( (process_data != NULL) && (process_data->bitmap != NULL) ) {
        p = process_data->bitmap;
        line_counter = HEIGHT;
        while ( line_counter-- ) {
            column_counter = WIDTH / 3;
            value = gradient_start_value++;
            while ( column_counter-- ) {
                *p++ = value++;
            }
            column_counter = WIDTH * 2 / 3;
            value = checkerboard_start_value;
            while ( column_counter-- ) {
                *p++ = value;
                value = ~value;
            }
            checkerboard_start_value = ~checkerboard_start_value;
        }
    } else status = -1; // parameter issue

    return status;
}

int pointer_data_processing_2(process_data_t * process_data) {
    int status = 0;

    if( (process_data != NULL) && (process_data->bitmap != NULL) ) {
        uint32_t counter = WIDTH * HEIGHT;
        uint8_t third_width = WIDTH / 3;
        uint8_t two_third_width = WIDTH - third_width;
        uint8_t first_value = 0;
        uint8_t first_row_value = 0;
        uint32_t third_counter = third_width;

        //lut
        uint8_t * p = (uint8_t *) malloc(WIDTH * HEIGHT * 2/3 * sizeof(uint8_t));
        uint8_t * pp = p;
        uint8_t x = (uint8_t) (WIDTH * HEIGHT * 2 / 3) / 2;

        while (counter != 0) {
            while (third_counter --) {
                *process_data->bitmap = first_value;
                first_value++;
                *process_data->bitmap++;
                counter--;
            }
            third_counter = two_third_width;
            while (third_counter --) {
                *process_data->bitmap = *pp;
                *pp++;
                *process_data->bitmap++;
                counter--;
            }
            third_counter = third_width;
            first_row_value++;
            first_value = first_row_value;
        }
    } else status = -1; //


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


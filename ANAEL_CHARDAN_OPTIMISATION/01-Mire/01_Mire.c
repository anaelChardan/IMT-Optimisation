#include <stdio.h>
#include <stdlib.h> // malloc
#include <string.h> // memset
#include <stdint.h> // uintxx_t types

#include "time_measure.h"
#include "../common/image_utils.h"

#define FRAME_VALUE 142

int basic_data_processing(process_data_t * process_data);
int faster_data_processing(process_data_t * process_data);

int init_process_data(process_data_t * process_data, int argc, const char * argv[]) {
	int status = 0;
	
	if( process_data != NULL ) {
		memset(process_data, 0, sizeof(process_data_t));
		// affect processing function
		process_data->data_processing_function = faster_data_processing;
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
	int pixel_changed;
	
	if( (process_data != NULL) && (process_data->bitmap != NULL) ) {
		// frame
		for( y = 0; y < HEIGHT; y++) {
			for(x = 0; x < WIDTH; x++) {
				pixel_changed = 0; // false
				if( y <= 1 ) { // upper frame
					process_data->bitmap[y * WIDTH + x] = FRAME_VALUE;
					pixel_changed = 1; // true
				}
				if( x <= 1 ) { // left frame
					process_data->bitmap[y * WIDTH + x] = FRAME_VALUE;
					pixel_changed = 1; // true
				}
				if( y >= (HEIGHT - 2) ) { // lower frame
					process_data->bitmap[y * WIDTH + x] = FRAME_VALUE;
					pixel_changed = 1; // true
				}
				if( x >= (WIDTH - 2) ) { // left frame
					process_data->bitmap[y * WIDTH + x] = FRAME_VALUE;
					pixel_changed = 1; // true
				}
				if( !pixel_changed ) {
					process_data->bitmap[y * WIDTH + x] = 0;
				}
			}
		}
		// inner oblic lines
		if( WIDTH > HEIGHT ) { // landscape
			// leftmost line
			for(y = 2; y < (HEIGHT - 2); y++) {
				process_data->bitmap[y * WIDTH + y] = 0xFF;
			}
			// righmost line
			for(y = 2; y < (HEIGHT - 2); y++) {
				process_data->bitmap[y * WIDTH + (WIDTH - y - 2)] = 0xFF;
			}
		} else { // portrait
			// leftmost line
			for(y = 2; y < (WIDTH - 2); y++) {
				process_data->bitmap[y * WIDTH + y] = 0xFF;
			}
			// righmost line
			for(y = 2; y < (WIDTH - 2); y++) {
				process_data->bitmap[y * WIDTH + (WIDTH - y - 2)] = 0xFF;
			}
		}
	} else status = -1; // parameter issue
	
	return status;
}

int faster_data_processing(process_data_t * process_data) {
	int status = 0;
	if( (process_data != NULL) && (process_data->bitmap != NULL) ) {

        uint8_t * p = process_data->bitmap;
		uint8_t high_lines_border = WIDTH*2;
        uint8_t line_counter = high_lines_border;

        //the higher border
		while (line_counter--) {
			*p = 142;
			p++;
		}

		uint8_t middle_frame_width = WIDTH - (2 * 2);
		uint8_t middle_frame_height = HEIGHT - (2*2);
        uint8_t middle_frame_width_cpt = middle_frame_width;

        //the middle
		while (middle_frame_height--) {
			*p = 142;
			p++;
			*p = 142;
            p++;
            while(middle_frame_width_cpt--) {
                *p = 0;
                p++;
            }
            middle_frame_width_cpt = middle_frame_width;
			*p = 142;
			p++;
			*p = 142;
			p++;
		}

        //the bottom border
        line_counter = high_lines_border;
		while (line_counter--) {
			*p = 142;
			p++;
		}

        // first line
        p = process_data->bitmap + (WIDTH * 2) + 2;
        line_counter = HEIGHT - 4;
        while(line_counter--) {
            *p = 255;
            p += WIDTH + 1;
        }

        // second line
        p = process_data->bitmap + (WIDTH * 3) - 3;
        line_counter = HEIGHT - 4;
        while(line_counter--) {
            *p = 255;
            p += WIDTH - 1;
        }

		return status;
	}

	return -1;
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


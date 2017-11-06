#include <stdio.h>
#include <stdlib.h> // malloc
#include <string.h> // memset
#include <stdint.h> // uintxx_t types

#include "common/time_mesure.h"
#include "common/image_utils.h"

int basic_data_processing(process_data_t * process_data); // 28s
int basic_data_processing_v2(process_data_t * process_data); // 22s
int faster_data_processing(process_data_t * process_data); // 1.2 s
int faster_data_processing_gradient_copy(process_data_t * process_data); // 0.8s

int init_process_data(process_data_t * process_data, int argc, const char * argv[]) {
	int status = 0;
	uint8_t gradient_value = 0;
	uint32_t i = 0;
	
	if( process_data != NULL ) {
		memset(process_data, 0, sizeof(process_data_t));
		// affect processing function
		process_data->data_processing_function = basic_data_processing;
		//process_data->data_processing_function = basic_data_processing_v2;
		//process_data->data_processing_function = faster_data_processing;
		//process_data->data_processing_function = faster_data_processing_gradient_copy;
		switch( argc ) {
			case 2:
				// allocate and initialize bitmap
				process_data->bitmap = (uint8_t *) malloc(WIDTH * HEIGHT * sizeof(uint8_t));
				if( process_data->bitmap ) {
					process_data->output_file_path = argv[1];
					for(i = 0; i < 0x100; i++)
						process_data->h_gradient[i] = gradient_value++;
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
				if( x == 0 )
					// first column : value = y
					process_data->bitmap[y * WIDTH + 0] = y;
				if( x == 1 )
					// second column : value = y + 1
					process_data->bitmap[y * WIDTH + 1] = y + 1;
				if( x == 2 )
					// third column : value = y + 2
					process_data->bitmap[y * WIDTH + 2] = y + 2;
				if( (x >= 3) && (x < (0x100 + 3)) )
					// horizontal gradient follows on next 256 columns
					process_data->bitmap[y * WIDTH + x] = x  - 3;
				if( x >= (0x100 + 3) )
					// vertical gradient follows on remaining columns
					process_data->bitmap[y * WIDTH + x] = 0xFF - y;
			}
		}
	} else status = -1; // parameter issue
	
	return status;
}


int basic_data_processing_v2(process_data_t * process_data) {
	int status = 0;
	uint32_t x = 0, y = 0;
	
	if( (process_data != NULL) && (process_data->bitmap != NULL) ) {
		// first column : value = y
		for( y = 0; y < HEIGHT; y++)
			process_data->bitmap[y * WIDTH + 0] = y;
		// second column : value = y + 1
		for( y = 0; y < HEIGHT; y++)
			process_data->bitmap[y * WIDTH + 1] = y + 1;
		// third column : value = y + 2
		for( y = 0; y < HEIGHT; y++)
			process_data->bitmap[y * WIDTH + 2] = y + 2;
		// horizontal gradient follows on next 256 columns
		for(x = 3; x < (0x100 + 3); x++)
			for(y = 0; y < HEIGHT; y++)
				process_data->bitmap[y * WIDTH + x] = x  - 3;
		// vertical gradient follows on remaining columns
		for(y = 0; y < HEIGHT; y++)
			for(x = (0x100 + 3); x < WIDTH; x++)
				process_data->bitmap[y * WIDTH + x] = 0xFF - y;
	} else status = -1; // parameter issue
	
	return status;
}


int faster_data_processing(process_data_t * process_data) {
	int status = 0;
	uint8_t * p = NULL, y = 0, h_pattern = 0, v_pattern = 0xFF;
	uint32_t line_counter = 0, counter = 0;
	
	if( (process_data != NULL) && (process_data->bitmap != NULL) ) {
		p = process_data->bitmap;
		line_counter = HEIGHT;
		while( line_counter-- ) {
			// first three pixels are line index
			*p++ = y++;
			*p++ = y;
			*p++ = y + 1;
			h_pattern = 0;
			counter = 0x100;
			while( counter-- )
				*p++ = h_pattern++;
			memset(p, v_pattern--, WIDTH - 0x103);
			p += WIDTH - 0x103;
		}
	} else status = -1; // parameter issue
	
	return status;
}


int faster_data_processing_gradient_copy(process_data_t * process_data) {
	int status = 0;
	uint8_t * p = NULL, y = 0, v_pattern = 0xFF, * h_gradient = NULL;
	uint32_t line_counter = 0;
	
	if( (process_data != NULL) && (process_data->bitmap != NULL) ) {
		p = process_data->bitmap;
		h_gradient = process_data->h_gradient;
		line_counter = HEIGHT;
		while( line_counter-- ) {
			// first three pixels are line index
			*p++ = y++;
			*p++ = y;
			*p++ = y + 1;
			memcpy(p, h_gradient, 0x100);
			p += 0x100;
			memset(p, v_pattern--, WIDTH - 0x103);
			p += WIDTH - 0x103;
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


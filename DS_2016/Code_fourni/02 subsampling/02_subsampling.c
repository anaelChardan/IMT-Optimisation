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
				// read BMP file
				status = read_bmp_file(argv[1], &process_data->source_header, &process_data->source_bitmap);
				if( status != -1 ) {
					switch( process_data->source_header.bits_per_pixel ) {
						case 8 :
							// allocate and initialize destination bitmap
							// prepare output header
							init_bmp_header(&process_data->destination_header);
							process_data->destination_header.width = process_data->source_header.width / 2;
							process_data->destination_header.height = process_data->source_header.height / 2;
							// allocate and initialize destination bitmap
							process_data->destination_bitmap = (uint8_t *) malloc(process_data->destination_header.width * process_data->destination_header.height * sizeof(uint8_t));
							if( process_data->destination_bitmap ) {
								process_data->output_file_path = suffixed_filename(argv[1], " (subsampled).bmp");
								status = 0;
							} else { // bitmap allocation issue
								fprintf(stderr, "Destination bitmap allocation failed");
								status = -2;
							}
							break;
						default:
							fprintf(stderr, "\"%s\" : has %d bits per pixels only 8 bpp is supported\n", argv[1], process_data->source_header.bits_per_pixel);
							status = -4;
							break;
					} // switch header.bits_per_pixel
				} else fprintf(stderr, "BMP read failed\n");
				break;
			default:
				printf("usage : %s <output BMP file path>\n", argv[0]);
				status = -3;
				break;
		}
	} else status = -1; // parameter issue
	
	return status;
}


int basic_data_processing(process_data_t * process_data) {
	int status = 0;
	uint32_t x = 0, y = 0, X = 0, Y = 0;
	double v = 0.0;
	
	if( (process_data != NULL) && (process_data->source_bitmap != NULL) && (process_data->destination_bitmap != NULL) ) {
		for(y = 0; y < process_data->destination_header.height; y++ ) {
			for( x = 0; x < process_data->destination_header.width; x++) {
				X = 2 * x;
				Y = 2 * y;
				v = (double) process_data->source_bitmap[Y * process_data->source_header.width + X];
				v += (double) process_data->source_bitmap[Y * process_data->source_header.width + X + 1];
				v += (double) process_data->source_bitmap[(Y + 1) * process_data->source_header.width + X];
				v += (double) process_data->source_bitmap[(Y + 1) * process_data->source_header.width + X + 1];
				v /= 4.0;
				process_data->destination_bitmap[y * process_data->destination_header.width + x] = (uint8_t) v;
			}
		}
	} else status = -1; // parameter issue
	
	return status;
}


int dispose_process_data(process_data_t * process_data) {
	int status = 0;
	
	if( (process_data != NULL) ) {
		if( process_data->source_bitmap != NULL ) {
			free(process_data->source_bitmap);
			process_data->source_bitmap = NULL;
		}
		if( process_data->destination_bitmap != NULL ) {
			if( process_data->output_file_path != NULL ) {
				// save bitmap in BMP format
				save_bitmap(process_data->output_file_path, &process_data->destination_header, process_data->destination_bitmap);
				free(process_data->output_file_path);
			}
			free(process_data->destination_bitmap);
			process_data->destination_bitmap = NULL;
		}
	} else status = -1; // parameter issue
	
	return status;
}


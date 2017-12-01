#include <stdio.h>
#include <stdlib.h> // malloc
#include <string.h> // memset
#include <stdint.h> // uintxx_t types

#include "time_measure.h"
#include "../common/image_utils.h"

#define __min(a,b) (((a) < (b)) ? a : b)
#define __max(a,b) (((a) < (b)) ? b : a)

int basic_data_processing(process_data_t * process_data);
int faster_data_processing(process_data_t * process_data);

int init_process_data(process_data_t * process_data, int argc, const char * argv[]) {
	int status = 0;
	uint32_t value1 = 0, value2 = 0;
	
	if( process_data != NULL ) {
		memset(process_data, 0, sizeof(process_data_t));
		// affect processing function
		process_data->data_processing_function = faster_data_processing;
		switch( argc ) {
			case 6:
				// read BMP file
				status = read_bmp_file(argv[1], &process_data->source_header, &process_data->source_bitmap);
				if( status != -1 ) {
					switch( process_data->source_header.bits_per_pixel ) {
						case 8 :
							printf("Input file %s is %u pixels wide and %u pixels tall\n", argv[1], process_data->source_header.width, process_data->source_header.height);
							sscanf(argv[2], "%u", &value1);
							sscanf(argv[4], "%u", &value2);
							process_data->x0 = __min(value1, value2);
							process_data->x1 = __max(value1, value2);
							sscanf(argv[3], "%u", &value1);
							sscanf(argv[5], "%u", &value2);
							process_data->y0 = __min(value1, value2);
							process_data->y1 = __max(value1, value2);
							if( process_data->x0 != 0 && process_data->x1 != 0 && process_data->y0 != 0 && process_data->y1 != 0 ) {
								if( process_data->x0 <= process_data->source_header.width && process_data->x1 <= process_data->source_header.width && process_data->y0 <= process_data->source_header.height && process_data->y1 <= process_data->source_header.height ) {
									// prepare output header
									init_bmp_header(&process_data->destination_header);
									process_data->destination_header.width = process_data->x1 - process_data->x0 + 1;
									process_data->destination_header.height = process_data->y1 - process_data->y0 + 1;
									// allocate and initialize destination bitmap
									process_data->destination_bitmap = (uint8_t *) malloc(process_data->destination_header.width * process_data->destination_header.height * sizeof(uint8_t));
									if( process_data->destination_bitmap ) {
										process_data->output_file_path = suffixed_filename(argv[1], " (cropped).bmp");
										status = 0;
									} else { // bitmap allocation issue
										fprintf(stderr, "Destination bitmap allocation failed");
										status = -5;
									}
								} else { // one coordinate is out of range
									fprintf(stderr, "One of x0=%u y0=%u x1=%u y1=%u is out of range (%u x %u)\n", process_data->x0, process_data->y1, process_data->x1, process_data->y1, process_data->source_header.width, process_data->source_header.height);
									status = -2;
								}
							} else { // one coordinate is null
								fprintf(stderr, "One of x0=%u y0=%u x1=%u y1=%u is 0\n", process_data->x0, process_data->y1, process_data->x1, process_data->y1);
								status = -2;
							}
							break;
						default:
							fprintf(stderr, "\"%s\" : has %d bits per pixels - only 8 bpp is supported\n", argv[1], process_data->source_header.bits_per_pixel);
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
	uint32_t x = 0, y = 0;
	
	if( (process_data != NULL) && (process_data->source_bitmap != NULL) && (process_data->destination_bitmap != NULL) ) {
		for(y = 0; y < process_data->source_header.height; y++ ) {
			for( x = 0; x < process_data->source_header.width; x++) {
				if( x >= process_data->x0 && x <= process_data->x1 && y >= process_data->y0 && y <= process_data->y1 ) {
					process_data->destination_bitmap[(y - process_data->y0) * process_data->destination_header.width + (x - process_data->x0)] = process_data->source_bitmap[y * process_data->source_header.width + x];
				}
			}
		}
	} else status = -1; // parameter issue
	
	return status;
}

int faster_data_processing(process_data_t * process_data) {
    if( (process_data != NULL) && (process_data->source_bitmap != NULL) && (process_data->destination_bitmap != NULL) ) {

        uint32_t ecart_droite = process_data->source_header.width - process_data->x1;
        uint32_t ecart_total = process_data->x0 + ecart_droite;
        uint32_t width = process_data->x1 - process_data->x0;
        uint32_t height = process_data->y1 - process_data->y0;
        uint32_t column_counter = width;
        uint32_t line_counter = height;

        uint8_t *p = process_data->source_bitmap;
        uint8_t *r = process_data->destination_bitmap;
        p += process_data->y0 * process_data->source_header.width + process_data->x0;

        while (line_counter--) {
            column_counter = width;
            while (column_counter--) {
                *r = *p;
                p++;
                r++;
            }
            p += ecart_total;
        }

        return 0;
    } return -1;
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


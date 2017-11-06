#include <stdio.h>
#include <stdlib.h> // malloc
#include <string.h> // memset
#include <stdint.h> // uintxx_t types

#include "common/08_time_mesure.h"
#include "common/image_utils.h"

int basic_data_processing(process_data_t * process_data);
int pointer_data_processing(process_data_t * process_data);
int pointer_data_processing_lut(process_data_t * process_data);

int init_process_data(process_data_t * process_data, int argc, const char * argv[]) {
	int status = 0;
	uint32_t i = 0;
	double v= 0.0;
	argv[1] = "/Users/anael/Developer/IMT/TestOptimisationDeCode/large_files/sea.bmp";
    argc = 2;
	if( process_data != NULL ) {
		memset(process_data, 0, sizeof(process_data_t));
		// affect processing function
		process_data->data_processing_function = basic_data_processing; // 117
//		process_data->data_processing_function = pointer_data_processing; // 105
//		process_data->data_processing_function = pointer_data_processing_lut; // 23 (23)
		switch( argc ) {
			case 2:
				// read BMP file
				status = read_bmp_file(argv[1], &process_data->source_header, &process_data->source_bitmap);
				if( status != -1 ) {
					switch( process_data->source_header.bits_per_pixel ) {
						case 8 :
							// allocate and initialize destination bitmap
							process_data->destination_bitmap = (uint8_t *) malloc(process_data->source_header.width * process_data->source_header.height * sizeof(uint8_t));
							if( process_data->destination_bitmap ) {
								process_data->output_file_path = suffixed_filename(argv[1], " (Lumiere Constraste).bmp");
								process_data->alpha = 1.253;
								process_data->beta = -15.7;
								for(i = 0; i < 256; i++) {
									v = (double) i;
									v = process_data->alpha * v + process_data->beta;
									if( v < 0 )
										v = 0;
									else if( v > 255 )
										v = 255;
								 process_data->lut[i] = (uint8_t) v;
								}

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
	uint32_t x = 0, y = 0;
	double v = 0.0;
	
	if( (process_data != NULL) && (process_data->source_bitmap != NULL) && (process_data->destination_bitmap != NULL) ) {
		for(y = 0; y < process_data->source_header.height; y++ ) {
			for( x = 0; x < process_data->source_header.width; x++) {
				v = (double) process_data->source_bitmap[y * process_data->source_header.width + x];
				v = process_data->alpha * v + process_data->beta;
				if( v < 0 )
					v = 0;
				else if( v > 255 )
					v = 255;
				process_data->destination_bitmap[y * process_data->source_header.width + x] = (uint8_t) v;
			}
		}
	} else status = -1; // parameter issue
	
	return status;
}


int pointer_data_processing(process_data_t * process_data) {
	int status = 0;
	uint8_t * source_pointer = NULL;
	uint8_t * destination_pointer = NULL;
	uint32_t counter = 0;
	double v = 0.0;
	
	if( (process_data != NULL) && (process_data->source_bitmap != NULL) && (process_data->destination_bitmap != NULL) ) {
		counter = process_data->source_header.width * process_data->source_header.height;
		//lut = process_data->negative_LUT;
		source_pointer = process_data->source_bitmap;
		destination_pointer = process_data->destination_bitmap;
		while( counter -- ) {
			v = (double) *source_pointer++;
			v = process_data->alpha * v + process_data->beta;
			if( v < 0 )
				v = 0;
			else if( v > 255 )
				v = 255;
			*destination_pointer++ = (uint8_t) v;
		}
	} else status = -1; // parameter issue
	
	return status;
}


int pointer_data_processing_lut(process_data_t * process_data) {
	int status = 0;
	uint8_t * source_pointer = NULL;
	uint8_t * destination_pointer = NULL;
	uint32_t counter = 0;
	uint8_t * lut = NULL;
	
	if( (process_data != NULL) && (process_data->source_bitmap != NULL) && (process_data->destination_bitmap != NULL) ) {
		counter = process_data->source_header.width * process_data->source_header.height;
		lut = process_data->lut;
		source_pointer = process_data->source_bitmap;
		destination_pointer = process_data->destination_bitmap;
		while( counter -- ) {
			*destination_pointer++ = process_data->lut[*source_pointer++];
//			*destination_pointer++ = lut[*source_pointer++];
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
				save_bitmap(process_data->output_file_path, &process_data->source_header, process_data->destination_bitmap);
				free(process_data->output_file_path);
			}
			free(process_data->destination_bitmap);
			process_data->destination_bitmap = NULL;
		}
	} else status = -1; // parameter issue
	
	return status;
}


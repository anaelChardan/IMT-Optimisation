#include <stdio.h>
#include <stdlib.h> // malloc
#include <string.h> // memset
#include <stdint.h> // uintxx_t types

#include "common/image_utils.h"

#define OPTIM_LEVEL 0

int main(int argc, const char * argv[])
{
	int status = -1;
	bmp_header_t header, copy_header;
	uint8_t * bitmap = NULL;
	char * copy_file_path = NULL;
	
	switch( argc ) {
		case 2 :
			status = read_bmp_file(argv[1], &header, &bitmap);
			if( status != -1 ) {
				copy_file_path = suffixed_filename(argv[1], " (copy).bmp");
				if( copy_file_path ) {
					double v = 0.0, alpha = 1.2, beta = -10.0;
#if OPTIM_LEVEL == 0
					 for(uint32_t y = 0; y < header.height; y++ ) {
						for(uint32_t x = 0; x < header.width; x++) {
							 v = (double) bitmap[y * header.width + x];
							 v = alpha * v + beta;
							 if( v < 0 )
								 v = 0;
							 else if( v > 255 )
								 v = 255;
							 bitmap[y * header.width + x] = (uint8_t) v;
						}
					 }
#elif OPTIM_LEVEL == 1
						uint32_t counter = header.width * header.height;
						uint8_t * p = bitmap + 0;
						while( counter-- != 0 ) {
							 v = (double) *p;
							 v = alpha * v + beta;
							 if( v < 0 )
									v = 0;
							 else if( v > 255 )
								 v = 255;
							 *p = (uint8_t) v;
							 p++;
							}
#elif OPTIM_LEVEL == 2
						// LUT version
						uint32_t counter = header.width * header.height;
						uint8_t * p = bitmap + 0;
						uint8_t * lut = (uint8_t) malloc(256 * sizeof(uint8_t));
						if( lut ) {
							uint32_t i = 0;
							for(i = 0; i < 256; i++) {
								 v = (double) i;
								 v = alpha * v + beta;
								 if( v < 0 )
									 v = 0;
								 else if( v > 255 )
									 v = 255;
								 lut[i] = (uint8_t) v;
							}
							
							while( counter-- != 0 ) {
								 *p = lut[*p];
								 p++;
							}
							
							free(lut);
						 } else fprintf(stderr, "LUT allocation failed\n");
#endif
					init_bmp_header(&copy_header);
					copy_header.width = header.width;
					copy_header.height = header.height;
					status = save_bitmap(copy_file_path, &copy_header, bitmap);
					free(copy_file_path);
				} else fprintf(stderr, "copy_file_path allocation failed\n");
				free(bitmap);
			}
			break;
		default:
			fprintf(stderr, "usage : %s <file path>\n", argv[0]);
			break;
	}
	
#ifdef WIN32
	system("pause");
#endif
	
	return 0;
}


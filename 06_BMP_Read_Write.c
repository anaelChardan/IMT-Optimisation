#include <stdio.h>
#include <stdlib.h> // malloc
#include <string.h> // memset
#include <stdint.h> // uintxx_t types

#include "common/image_utils.h"



int main(int argc, const char * argv[])
{
	int status = -1;
	bmp_header_t header, copy_header;
	uint8_t * bitmap = NULL;
	char * copy_file_path = NULL;
	uint32_t counter = 0, row_counter = 0;
	uint8_t * p = NULL;

	argv[1] = "/Users/anael/Developer/IMT/TestOptimisationDeCode/sample/chat.bmp";
	argc = 2;

	switch( argc ) {
		case 2 :
			status = read_bmp_file(argv[1], &header, &bitmap);
			if( status != -1 ) {
				if( bitmap != NULL ) {
					adjust_pixel(bitmap, header.width, header.height, 0.7, 15);
				}

				copy_file_path = suffixed_filename(argv[1], "_copy.bmp");
				if( copy_file_path ) {
					
					// do something with picture stored at bitmap…
					
					init_bmp_header(&copy_header);
					copy_header.width = header.width;
					copy_header.height = header.height;
					copy_header.bits_per_pixel = header.bits_per_pixel;
					status = save_bitmap(copy_file_path, &copy_header, bitmap);
					free(copy_file_path);
				} else fprintf(stderr, "copy_file_path allocation failed");
				free(bitmap);
			}
			break;
		default:
			fprintf(stderr, "usage : %s <file path>", argv[0]);
			break;
	}
	
#ifdef WIN32
	system("pause");
#endif
	
	return 0;
}


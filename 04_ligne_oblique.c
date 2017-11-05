#include <stdio.h>
#include <stdint.h> // uintxx_t
#include <stdlib.h> // malloc

#define WIDTH 11
#define HEIGHT 9

int main(int argc, const char * argv[])
{
	uint8_t * bitmap = NULL;
	uint8_t * p = NULL;
	uint32_t counter = 0, row_counter = 0;
	
	bitmap = (uint8_t *) malloc(WIDTH * HEIGHT * sizeof(uint8_t));
	if( bitmap != NULL ) {
		
		// set bitmap content
		counter = WIDTH * HEIGHT;
		p = bitmap;
		while( counter-- ) {
			*p++ = 0;
		}
		
		// draw line x = y
		counter = HEIGHT < WIDTH ? HEIGHT : WIDTH;
		p = bitmap;
		while( counter-- ) {
			*p = 0x12;
			p += WIDTH + 1;
		}
		
		// display bitmap content
		counter = HEIGHT;
		p = bitmap;
		while( counter-- ) {
			row_counter = WIDTH;
			while( row_counter-- ) {
				printf("0x%02x ", *p++);
			}
			printf("\n");
		}
		
		free(bitmap);
	} else
		fprintf(stderr, "bitmap allocation failed\n");
	
#ifdef WIN32
	system("pause");
#endif
	
	return 0;
}


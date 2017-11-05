#include <stdio.h>
#include <stdint.h> // uintxx_t
#include <stdlib.h> // malloc

#define WIDTH 7
#define HEIGHT 5

int main(int argc, const char * argv[])
{
	uint8_t * bitmap = NULL;
	uint8_t * p = NULL;
	uint32_t x = 0, y = 0;
	uint32_t counter = 0;
	
	bitmap = (uint8_t *) malloc(WIDTH * HEIGHT * sizeof(uint8_t));
	if( bitmap != NULL ) {
		
		// set bitmap content
		counter = WIDTH * HEIGHT;
		p = bitmap;
		while( counter-- ) {
			*p++ = 0;
		}
		
		// display bitmap content
		for(y = 0; y < HEIGHT; y++) {
			for(x = 0; x < WIDTH; x++) {
				printf("0x%02x ", bitmap[y * WIDTH + x]);
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


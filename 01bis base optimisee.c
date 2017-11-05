#include <stdio.h>
#include <stdint.h> // uintxx_t
#include <stdlib.h> // malloc

#define WIDTH 7
#define HEIGHT 5

int main(int argc, const char * argv[])
{
	uint8_t * bitmap = NULL;
	uint8_t * p = NULL;
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
		counter = WIDTH * HEIGHT;
		p = bitmap;
		while( counter-- ) {
			printf("0x%02x ", *p);
			if( counter % WIDTH == 0 )
				printf("\n");
			p++;
		}

		free(bitmap);
	} else
		fprintf(stderr, "bitmap allocation failed\n");
	
#ifdef WIN32
	system("pause");
#endif
	
	return 0;
}


#include <stdio.h>
#include <stdlib.h> // malloc
#include <string.h> // memset
#include <stdint.h> // uintxx_t types
#include <fcntl.h> // open
#include <unistd.h> // write

#define WIDTH 100
#define HEIGHT 160

int main (int argc, const char * argv[])
{
	uint8_t * bitmap = NULL;
	uint8_t * p = NULL;
	uint32_t counter = 0;
	uint32_t x0 = WIDTH / 4;
	int fd = -1;
	size_t bytes_to_write = 0;
	ssize_t bytes_written = 0;
	
	bitmap = (uint8_t *) malloc(WIDTH * HEIGHT * sizeof(uint8_t));
	if( bitmap != NULL ) {
		// initialize bitmap to black
		memset(bitmap, 0, WIDTH * HEIGHT * sizeof(uint8_t));
		
		// set middle point to white
		bitmap[HEIGHT / 2 * WIDTH + WIDTH / 2] = 0xFF;
		
		// draw white line at y = height/3
		memset(bitmap + HEIGHT / 3 * WIDTH, 0xFE, WIDTH * sizeof(uint8_t));
		
		// draw line at x = x0
		p = bitmap + x0;
		counter = HEIGHT;
		while( counter-- ) {
			*p = 0x80;
			p += WIDTH;
		}
		
		// draw line x = y
		p = bitmap;
		counter = WIDTH > HEIGHT ? HEIGHT : WIDTH;
		while( counter-- ) {
			*p = 0xC0;
			p += WIDTH + 1;
		}
		
		fd = open("premier test.data", O_CREAT | O_TRUNC | O_WRONLY | O_APPEND
#ifdef WIN32
							| O_BINARY
#else // WIN32
							, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH
#endif // WIN32
							);
		if( fd != -1 ) {
			bytes_to_write = HEIGHT * WIDTH * sizeof(uint8_t);
			bytes_written = write(fd, bitmap, bytes_to_write);
			if( bytes_written == bytes_to_write ) {
				printf("data sucessfully written\n");
			} else printf("data write failed\n");
			close(fd);
		} else printf("file open failed\n");
		free(bitmap);
	} else printf("bitmap allocation failed\n");
	
#ifdef WIN32
	system("pause");
#endif
	
	return 0;
}


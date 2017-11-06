#include <stdio.h>
#include <stdlib.h> // malloc
#include <string.h> // memset
#include <stdint.h> // uintxx_t types
#include <fcntl.h> // open
#include <unistd.h> // write

#include "bmp.h"

int read_bmp_header(int fd, bmp_header_t * header) {
	size_t bytes_to_read = 0;
	ssize_t bytes_read = 0;
	
	bytes_to_read = sizeof(char[2]); bytes_read = read(fd, &(header->file_type), bytes_to_read);
	if( bytes_read != bytes_to_read ) { printf("file_type read failed\n"); return -1; }
	if( (header->file_type[0] != 'B') || (header->file_type[1] != 'M') ) { printf("file content does not look like BMP format\n"); return -1; }
	
	bytes_to_read = sizeof(uint32_t); bytes_read = read(fd, &(header->file_size), bytes_to_read);
	if( bytes_read != bytes_to_read ) { printf("file_size read failed\n"); return -1; }
	
	bytes_to_read = sizeof(uint32_t); bytes_read = read(fd, &(header->reserved0), bytes_to_read);
	if( bytes_read != bytes_to_read ) { printf("reserved0 read failed\n"); return -1; }
	
	bytes_to_read = sizeof(uint32_t); bytes_read = read(fd, &(header->bitmap_offset), bytes_to_read);
	if( bytes_read != bytes_to_read ) { printf("bitmap_offset read failed\n"); return -1; }
	
	bytes_to_read = sizeof(uint32_t); bytes_read = read(fd, &(header->header_size), bytes_to_read);
	if( bytes_read != bytes_to_read ) { printf("header_size read failed\n"); return -1; }
	
	bytes_to_read = sizeof(uint32_t); bytes_read = read(fd, &(header->width), bytes_to_read);
	if( bytes_read != bytes_to_read ) { printf("width read failed\n"); return -1; }
	
	bytes_to_read = sizeof(uint32_t); bytes_read = read(fd, &(header->height), bytes_to_read);
	if( bytes_read != bytes_to_read ) { printf("height read failed\n"); return -1; }
	
	bytes_to_read = sizeof(uint16_t); bytes_read = read(fd, &(header->planes), bytes_to_read);
	if( bytes_read != bytes_to_read ) { printf("planes read failed\n"); return -1; }
	
	bytes_to_read = sizeof(uint16_t); bytes_read = read(fd, &(header->bits_per_pixel), bytes_to_read);
	if( bytes_read != bytes_to_read ) { printf("bits_per_pixel read failed\n"); return -1; }
	
	bytes_to_read = sizeof(uint32_t); bytes_read = read(fd, &(header->compression), bytes_to_read);
	if( bytes_read != bytes_to_read ) { printf("compression read failed\n"); return -1; }
	
	bytes_to_read = sizeof(uint32_t); bytes_read = read(fd, &(header->bitmap_size), bytes_to_read);
	if( bytes_read != bytes_to_read ) { printf("bitmap_size read failed\n"); return -1; }
	
	bytes_to_read = sizeof(uint32_t); bytes_read = read(fd, &(header->h_resolution), bytes_to_read);
	if( bytes_read != bytes_to_read ) { printf("h_resolution read failed\n"); return -1; }
	
	bytes_to_read = sizeof(uint32_t); bytes_read = read(fd, &(header->v_resolution), bytes_to_read);
	if( bytes_read != bytes_to_read ) { printf("v_resolution read failed\n"); return -1; }
	
	bytes_to_read = sizeof(uint32_t); bytes_read = read(fd, &(header->colors_used), bytes_to_read);
	if( bytes_read != bytes_to_read ) { printf("colors_used read failed\n"); return -1; }
	
	bytes_to_read = sizeof(uint32_t); bytes_read = read(fd, &(header->important_colors), bytes_to_read);
	if( bytes_read != bytes_to_read ) { printf("important_colors read failed\n"); return -1; }
	
	return 0;
}

int init_bmp_header(bmp_header_t * header) {
	
	header->file_type[0] = 'B';
	header->file_type[1] = 'M';
	header->file_size = 0;
	header->reserved0 = 0;
	header->bitmap_offset = 54 + 256 * sizeof(uint32_t);
	header->header_size = 40;
	header->width = 0;
	header->height = 0;
	header->planes = 1;
	header->bits_per_pixel = 8;
	header->compression = 0;
	header->bitmap_size = 0;
	header->h_resolution = 0xb13;
	header->v_resolution = 0xb13;
	header->colors_used = 256;
	header->important_colors = 256;
	
	return 0;
}

int write_bmp_header(int fd, bmp_header_t * header) {
	size_t bytes_to_write = 0;
	ssize_t bytes_written = 0;
	uint32_t indexed_color = 0, counter = 256;
	uint32_t * color_data = NULL, * palette = NULL, * c = NULL;
	
	if( lseek(fd, 0, SEEK_SET) == -1 ) { // header should be at beginning of file
		printf("lseek failed\n"); return -1;}
	
	if( header->bits_per_pixel >= 24 ) {
		header->bitmap_offset = 54;
		if( header->bits_per_pixel == 32 ) {
			header->compression = 3;
			header->colors_used = 0;
			header->important_colors = 0;
			header->bitmap_offset += BMP_COLOR_DATA_LEN;
		}
	}
	
	// FIXME : the following is highly inaccurate, in case of bits per pixel < 8, compression, etc...
	header->bitmap_size = ((header->width * header->bits_per_pixel / 8 + 3) / 4) * 4 * header->height * header->planes;
	header->file_size = header->bitmap_size + header->bitmap_offset;
	
	bytes_to_write = sizeof(char[2]); bytes_written = write(fd, &(header->file_type), bytes_to_write);
	if( bytes_written != bytes_to_write ) { printf("file_type write failed\n"); return -1; }
	
	bytes_to_write = sizeof(uint32_t); bytes_written = write(fd, &(header->file_size), bytes_to_write);
	if( bytes_written != bytes_to_write ) { printf("file_size write failed\n"); return -1; }
	
	bytes_to_write = sizeof(uint32_t); bytes_written = write(fd, &(header->reserved0), bytes_to_write);
	if( bytes_written != bytes_to_write ) { printf("reserved0 write failed\n"); return -1; }
	
	bytes_to_write = sizeof(uint32_t); bytes_written = write(fd, &(header->bitmap_offset), bytes_to_write);
	if( bytes_written != bytes_to_write ) { printf("bitmap_offset write failed\n"); return -1; }
	
	bytes_to_write = sizeof(uint32_t); bytes_written = write(fd, &(header->header_size), bytes_to_write);
	if( bytes_written != bytes_to_write ) { printf("header_size write failed\n"); return -1; }
	
	bytes_to_write = sizeof(uint32_t); bytes_written = write(fd, &(header->width), bytes_to_write);
	if( bytes_written != bytes_to_write ) { printf("width write failed\n"); return -1; }
	
	bytes_to_write = sizeof(uint32_t); bytes_written = write(fd, &(header->height), bytes_to_write);
	if( bytes_written != bytes_to_write ) { printf("height write failed\n"); return -1; }
	
	bytes_to_write = sizeof(uint16_t); bytes_written = write(fd, &(header->planes), bytes_to_write);
	if( bytes_written != bytes_to_write ) { printf("planes write failed\n"); return -1; }
	
	bytes_to_write = sizeof(uint16_t); bytes_written = write(fd, &(header->bits_per_pixel), bytes_to_write);
	if( bytes_written != bytes_to_write ) { printf("bits_per_pixel write failed\n"); return -1; }
	
	bytes_to_write = sizeof(uint32_t); bytes_written = write(fd, &(header->compression), bytes_to_write);
	if( bytes_written != bytes_to_write ) { printf("compression write failed\n"); return -1; }
	
	bytes_to_write = sizeof(uint32_t); bytes_written = write(fd, &(header->bitmap_size), bytes_to_write);
	if( bytes_written != bytes_to_write ) { printf("bitmap_size write failed\n"); return -1; }
	
	bytes_to_write = sizeof(uint32_t); bytes_written = write(fd, &(header->h_resolution), bytes_to_write);
	if( bytes_written != bytes_to_write ) { printf("h_resolution write failed\n"); return -1; }
	
	bytes_to_write = sizeof(uint32_t); bytes_written = write(fd, &(header->v_resolution), bytes_to_write);
	if( bytes_written != bytes_to_write ) { printf("v_resolution write failed\n"); return -1; }
	
	bytes_to_write = sizeof(uint32_t); bytes_written = write(fd, &(header->colors_used), bytes_to_write);
	if( bytes_written != bytes_to_write ) { printf("colors_used write failed\n"); return -1; }
	
	bytes_to_write = sizeof(uint32_t); bytes_written = write(fd, &(header->important_colors), bytes_to_write);
	if( bytes_written != bytes_to_write ) { printf("important_colors write failed\n"); return -1; }
	
	switch( header->bits_per_pixel ) {
		case 8 : // generate gray colored palette (ie R=G=B)
			palette = (uint32_t *) malloc(256 * sizeof(uint32_t));
			if( palette ) {
				c = palette;
				while( counter-- ) {
					//palette[255 - counter] = indexed_color;
					*c++ = indexed_color;
					indexed_color += 0x00010101;
				}
				bytes_to_write = 256 * sizeof(uint32_t);
				bytes_written = write(fd, palette, bytes_to_write);
				if( bytes_written != bytes_to_write )
					printf("color data write failed\n");
				free(palette);
			} else printf("Color data allocation failed\n");
			break;
		case 32 : // some gamma and alpha channel stuff shamelessly copied from http://en.wikipedia.org/wiki/BMP_file_format
			color_data = (uint32_t *) malloc(BMP_COLOR_DATA_LEN);
			if( color_data ) {
				memset(color_data, 0, BMP_COLOR_DATA_LEN);
				color_data[0] = 0x00FF0000;
				color_data[1] = 0x0000FF00;
				color_data[2] = 0x000000FF;
				color_data[3] = 0xFF000000;
				color_data[4] = 0x57696E20;
				bytes_to_write = BMP_COLOR_DATA_LEN;
				bytes_written = write(fd, color_data, bytes_to_write);
				if( bytes_written != bytes_to_write )
					printf("color data write failed\n");
				free(color_data);
			} else printf("Color data allocation failed\n");
			break;
	}	
	return 0;
}

int read_bmp_file_8(int fd, bmp_header_t * header, uint8_t ** bitmap) {
	int status = 0;
	size_t bytes_to_read = 0;
	ssize_t bytes_read = 0;
	uint8_t * p = NULL, * pline = NULL, filler[3], temp = 0;
	uint32_t line_counter = header->height, filler_byte_count = 0, row_byte = 0, counter = 0;
	
	status = (int) lseek(fd, header->bitmap_offset, SEEK_SET);
	if( status != -1 ) {
		status = 0;
		row_byte = header->width * (header->bits_per_pixel >> 3);
		switch( header->bits_per_pixel ) {
			case 8:
			case 24:
			case 32:
				*bitmap = (uint8_t *) malloc(header->height * row_byte);
				if( *bitmap ) {
					pline = *bitmap + (header->height - 1) * row_byte;
					/*if( row_byte % 4 )
					 filler_byte_count = 4 - (row_byte % 4);
					 else
					 filler_byte_count = 0;*/
					filler_byte_count = ((row_byte + 3) / 4) * 4 - row_byte;
					while( (line_counter--) && (status != -1) ) {
						bytes_to_read = row_byte;
						p = pline;
						do {
							bytes_read = read(fd, p, bytes_to_read);
							if( bytes_read > 0 ) {
								p += bytes_read;
								bytes_to_read -= bytes_read;
							}
						} while( (bytes_read > 0) && (bytes_to_read) );
						if( bytes_to_read == 0 ) {
							pline -= row_byte;
							if( filler_byte_count ) {
								bytes_to_read = filler_byte_count;
								bytes_read = read(fd, filler, bytes_to_read);
								if( bytes_read == bytes_to_read ) {
								} else printf("filler read failed\n");
							}						
						} else {
							printf("[read_bmp_file_8] data read failed : bytes_to_read=%ld bytes_read=%ld\n", bytes_to_read, bytes_read);
							status = -1;		
						}
					}
				} else printf("[read_bmp_file_8] bitmap allocation failed");
				break;
			default:
				printf("%d pixels bitmap format is not supported", header->bits_per_pixel);
				status = -1;
				break;
		}
		switch( header->bits_per_pixel ) {
			case 24: // transform data big endian-way
				p = *bitmap;
				counter = header->width * header->height; // in pixel
				while( counter-- ) {
					temp = *p;
					*p = p[2];
					p[2] = temp;
					p += 3;
				}
				break;
		}
	} else printf("lseek failed\n");
	
	return status;
}

int write_bmp_file_8(int fd, bmp_header_t * header, uint8_t * bitmap) {
	int status = 0;
	size_t bytes_to_write = 0;
	ssize_t bytes_written = 0;
	uint8_t * p = NULL, * pline = NULL, filler[3], temp = 0;
	uint32_t line_counter = header->height, filler_byte_count = 0, row_byte = 0, counter = 0;
	
	status = (int) lseek(fd, header->bitmap_offset, SEEK_SET);
	if( status != -1 ) {
		row_byte = header->width * (header->bits_per_pixel >> 3);
		switch( header->bits_per_pixel ) {
			case 24: // transform data little endian-way
				p = bitmap;
				counter = header->width * header->height; // in pixel
				while( counter-- ) {
					temp = *p;
					*p = p[2];
					p[2] = temp;
					p += 3;
				}
				break;
		}
		switch( header->bits_per_pixel ) {
			case 8:
			case 24:
			case 32:
				pline = bitmap + (header->height - 1) * row_byte;
				filler_byte_count = ((row_byte + 3) / 4) * 4 - row_byte;
				while( (line_counter--) && (status != -1) ) {
					bytes_to_write = row_byte;
					p = pline;
					do {
						bytes_written = write(fd, p, bytes_to_write);
						if( bytes_written > 0 ) {
							p += bytes_written;
							bytes_to_write -= bytes_written;
						}
					} while( (bytes_written > 0) && (bytes_to_write) );
					if( bytes_to_write == 0 ) {
						pline -= row_byte;
						if( filler_byte_count ) {
							bytes_to_write = filler_byte_count;
							bytes_written = write(fd, filler, bytes_to_write);
							if( bytes_written == bytes_to_write ) {
							} else printf("[write_bmp_file_8] filler write failed\n");
						}						
					} else {
						printf("[write_bmp_file_8] data write failed : bytes_to_write=%ld bytes_written=%ld\n", bytes_to_write, bytes_written);
						status = -1;
					}
				}
				break;
			default:
				printf("%d pixels bitmap format is not supported", header->bits_per_pixel);
				status = -1;
				break;
		}
		switch( header->bits_per_pixel ) {
			case 24: // transform data to orginal organization
				p = bitmap;
				counter = header->width * header->height; // in pixel
				while( counter-- ) {
					temp = *p;
					*p = p[2];
					p[2] = temp;
					p += 3;
				}
				break;
		}
	} else printf("lseek failed\n");
	
	return status;
}


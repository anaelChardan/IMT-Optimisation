#ifndef __IMAGE_UTILS__
#define __IMAGE_UTILS__

#include <stdio.h>
#include <stdlib.h> // malloc
#include <string.h> // memset
#include <stdint.h> // uintxx_t types
#include <fcntl.h> // open
#include <unistd.h> // write

#include "bmp.h"

int gain_contrast(uint8_t * bitmap, uint32_t width, uint32_t height, int16_t contrast, float gain);
int get_histogramm(uint8_t * bitmap, uint32_t width, uint32_t height, uint32_t * histogramm);
int get_integral_histogramm(uint32_t * histogramm, uint32_t * integral_histogramm);
int get_histogramm_extrema(uint32_t * histogramm, uint32_t threshold, uint8_t * pmin, uint8_t * pmax);
int save_histogramm_bitmap(uint32_t * histogramm, const char * file_name, const char * suffix);
char * suffixed_filename_gain_contrast(const char * file_name, int16_t gain, int16_t contrast);
char * suffixed_filename(const char * file_name, char * suffix);
int read_bmp_file(const char * file_path, bmp_header_t * pheader, uint8_t ** pbitmap);
int save_bitmap(const char * file_name, bmp_header_t * pheader, uint8_t * bitmap);
int apply_lookup_table(uint8_t * bitmap, uint32_t width, uint32_t height, uint8_t * lut);
int write_diag(uint8_t * bitmap, uint32_t width, uint32_t height);
int write_line(uint8_t * bitmap, uint32_t width, uint32_t height, uint32_t line);
int write_column(uint8_t * bitmap, uint32_t width, uint32_t height, uint32_t colum);
int horizontal_mirror(uint8_t * bitmap, uint32_t width, uint32_t height);
int vertical_mirror(uint8_t * bitmap, uint32_t width, uint32_t height);

#endif // !__IMAGE_UTILS__
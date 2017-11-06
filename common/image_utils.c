#include "image_utils.h"

int gain_contrast(uint8_t * bitmap, uint32_t width, uint32_t height, int16_t contrast, float gain) {
	int status = 0;
	uint32_t counter = width * height;
	uint8_t * pixel = bitmap;
	float temp = 0.0f;
	
	while( counter-- ) {
		temp = ((float) *pixel) * gain + (float) contrast;
		if( temp < 0.f )
			temp = 0.f;
		else if( temp > 255.f )
			temp = 255.f;
		*pixel++ = (uint8_t) temp;
	}
	
	return status;
}

int get_histogramm(uint8_t * bitmap, uint32_t width, uint32_t height, uint32_t * histogramm) {
	int status = 0;
	uint32_t counter = width * height;
	uint8_t * pixel = bitmap;
	
	memset(histogramm, 0, 0x100 * sizeof(uint32_t));
	while( counter-- ) {
		//uint8_t n = *pixel;
		//histogramm[n] = histogramm[n] + 1;
		//pixel++;
		histogramm[*pixel++]++;
	}
	
	return status;
}

int get_integral_histogramm(uint32_t * histogramm, uint32_t * integral_histogramm) {
	int status = 0;
	uint16_t level = 0;
	
	integral_histogramm[0] = histogramm[0];
	for(level = 1; level < 0x100; level++) {
		integral_histogramm[level] = integral_histogramm[level - 1] + histogramm[level];
	}
	
	return status;
}

int get_histogramm_extrema(uint32_t * histogramm, uint32_t threshold, uint8_t * pmin, uint8_t * pmax) {
	int status = 0;
	uint32_t counter = 0;
	uint8_t level = 0;
	
	do {
		counter += histogramm[level];
		level++;
	} while( (level < 255) && (counter < threshold)  );
	*pmin = level - 1;
	
	level = 0xFF;
	counter = 0;
	do {
		counter += histogramm[level];
		level--;
	} while( (level > 0) && (counter < threshold)  );
	*pmax = level + 1;
	
	return status;
}

int save_histogramm_bitmap(uint32_t * histogramm, const char * file_name, const char * suffix) {
	int status = 0;
	uint8_t * bitmap = NULL;
	uint8_t * p = NULL;
	bmp_header_t header;
	uint16_t level = 0;
	uint32_t counter = 0;
	char * histogramm_file_name = NULL;
	char * built_suffix = NULL;
	float max_value = 0.f;
	
	status = init_bmp_header(&header);
	if( status != -1 ) {
		header.width = 0x100;
		header.height = 100;
		bitmap = (uint8_t *) malloc(header.width * header.height * sizeof(uint8_t));
		if( bitmap ) {
			memset(bitmap, 0, header.width * header.height);
			// first find maximum value to be able to handle percentages
			for(level = 0; level < 0x100; level++)
				if( histogramm[level] > ((uint32_t) max_value) )
					max_value = (float) histogramm[level];
			// draw white vertical lines, column-per-column
			for(level = 0; level < 0x100; level++) {
				p = bitmap + level; // start at top of column
				counter = 100.0 * (1.0f - ((float) histogramm[level]) / max_value);
				while( counter-- ) {
					*p = 0xFF;
					p += header.width;
				}
			}
			// save bitmap to file
			if( suffix && *suffix ) {
				built_suffix = (char *) malloc(strlen(" (histogram).bmp") + strlen(suffix));
				if( built_suffix ) {
					sprintf(built_suffix, " (histogram%s).bmp", suffix);
					histogramm_file_name = suffixed_filename(file_name, built_suffix);
				} else fprintf(stderr, "[save_histogramm_bitmap] built_suffix allocation failed\n");
			}
			if( histogramm_file_name == NULL )
				histogramm_file_name = suffixed_filename(file_name, " (histogram).bmp");
			if( histogramm_file_name ) {
				status = save_bitmap(histogramm_file_name, &header, bitmap);
				free(histogramm_file_name);
			} else fprintf(stderr, "[save_histogramm_bitmap] suffixed_filename failed\n");
			free(bitmap);
		} else fprintf(stderr, "[save_histogramm_bitmap] bitmap allocation failed\n");
	} else fprintf(stderr, "[save_histogramm_bitmap] init_bmp_header failed\n");
	
	return status;
}

char * suffixed_filename_gain_contrast(const char * file_name, int16_t gain, int16_t contrast) {
	char * out_file_name = NULL;
	char * suffix = NULL;
	
	suffix = (char *) malloc(strlen(" (gain=123 contrast=-123).bmp") + 1);
	if( suffix ) {
		sprintf(suffix, " (gain=%03d contrast=%03d).bmp", gain, contrast);
		out_file_name = suffixed_filename(file_name, suffix);
		free(suffix);
	} else fprintf(stderr, "output file name allocation failed\n");
	
	return out_file_name;
}

char * suffixed_filename(const char * file_name, char * suffix) {
	char * out_file_name = NULL;
	char * file_name_without_extension = NULL;
	char * extension = NULL;
	
	file_name_without_extension = (char *) malloc(strlen(file_name) + 1);
	if( file_name_without_extension ) {
		strcpy(file_name_without_extension, file_name);
		extension = strrchr(file_name_without_extension, '.');
		if( extension ) { // any extension will be ignored
			*extension = 0;
		}
		out_file_name = (char *) malloc(strlen(file_name_without_extension) + strlen(suffix) + 1);
		if( out_file_name ) {
			sprintf(out_file_name, "%s%s", file_name_without_extension, suffix);
		} else fprintf(stderr, "output file name allocation failed\n");
		free(file_name_without_extension);
	} else fprintf(stderr, "File name without extension allocation failed\n");
	
	return out_file_name;
}

int read_bmp_file(const char * file_path, bmp_header_t * pheader, uint8_t ** pbitmap) {
	int status = -1, fd = -1;
	
			fd = open(file_path, O_RDONLY
#ifdef WIN32
								| O_BINARY
#endif // WIN32
								);
			if( fd != -1 ) {
				status = read_bmp_header(fd, pheader);
				if( status != -1 ) {
					status = read_bmp_file_8(fd, pheader, pbitmap);
					if( status != -1 ) {
						
					} else fprintf(stderr, "BMP content read failed\n");
				} else fprintf(stderr, "BMP header read failed\n");
				close(fd);
			} else fprintf(stderr, "file open failed\n");
	
	return status;
}

int save_bitmap(const char * file_name, bmp_header_t * pheader, uint8_t * bitmap) {
	int status = -1;
	int write_fd = -1;
	if( file_name != NULL && *file_name != 0 ) {
		write_fd = open(file_name, O_CREAT | O_TRUNC | O_WRONLY | O_APPEND
#ifdef WIN32
										| O_BINARY
#else // WIN32
										, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH
#endif // WIN32
										);
		if( write_fd != -1 ) {
			status = write_bmp_header(write_fd, pheader);
			if( status != -1 ) {
				status = write_bmp_file_8(write_fd, pheader, bitmap);
				if( status != -1 ) {
					printf("BMP file \"%s\" written\n", file_name);
				} else fprintf(stderr, "BMP file \"%s\" content write failed\n", file_name);
			} else fprintf(stderr, "BMP file \"%s\"  header write failed\n", file_name);
			close(write_fd);
		} else fprintf(stderr, "BMP file \"%s\"  open failed\n", file_name);
	} else fprintf(stderr, "output file name not set\n");
	
	return 0;
}

int apply_lookup_table(uint8_t * bitmap, uint32_t width, uint32_t height, uint8_t * lut) {
	int status = 0;
	uint32_t counter = width * height;
	uint8_t * pixel = bitmap;
	
	while( counter-- ) {
		*pixel = lut[*pixel];
		pixel++;
	}
	
	return status;
}


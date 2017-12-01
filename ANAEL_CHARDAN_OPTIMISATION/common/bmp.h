#ifndef _BMP_
#define _BMP_

#define BMP_COLOR_DATA_LEN ((8 * 4 + 0x24) * sizeof(uint8_t))

struct __bmp_header {
    // file header
    char            file_type[2];
    uint32_t        file_size;
    uint32_t        reserved0;
    uint32_t        bitmap_offset;
    // picture header
    uint32_t        header_size;
    uint32_t        width;
    uint32_t        height;
    uint16_t        planes;
    uint16_t        bits_per_pixel;
    uint32_t        compression;
    uint32_t        bitmap_size;
    uint32_t        h_resolution;
    uint32_t        v_resolution;
    uint32_t        colors_used;
    uint32_t        important_colors;
};

typedef struct __bmp_header bmp_header_t;

int read_bmp_header(int fd, bmp_header_t * header);
int init_bmp_header(bmp_header_t * header);
int write_bmp_header(int fd, bmp_header_t * header);
int read_bmp_file_8(int fd, bmp_header_t * header, uint8_t ** bitmap);
int write_bmp_file_8(int fd, bmp_header_t * header, uint8_t * bitmap);


#endif // _BMP_ 

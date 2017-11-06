#include <stdio.h> // printf
#include <stdint.h> // uintxx_t
#include <stdlib.h> // malloc

#define WIDTH 7
#define HEIGHT 5

int main(int argc, const char * argv[])
{
    //donnee dans la pile
    uint8_t bitmap[HEIGHT][WIDTH];
    uint32_t x = 0, y = 0;

    // display bitmap content
    for(y = 0; y <= HEIGHT; y++) {
        for(x = 0; x < WIDTH; x++) {
            printf("0x%02x ", bitmap[y][x]);
        }
        printf("\n");
    }

#ifdef WIN32
    system("pause");
#endif

    return 0;
}


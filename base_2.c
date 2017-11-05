#include <stdio.h> // printf
#include <stdint.h> // uintxx_t
#include <stdlib.h> // malloc

#define WIDTH 7
#define HEIGHT 5

///TOUJOURS FAIRE CONDITION INITIALE / ACTION / ACTION POUR ETAPE SUIVANTE / COMBIEN DE FOIS

int main(int argc, const char * argv[])
{
    //donnees dans le tas
    //
    uint8_t * bitmap = NULL;
    uint32_t x = 0, y = 0, n=0, n1=0, lig=0, col=0;
    uint8_t * p = NULL;
    uint8_t temp = NULL;
    uint8_t * p1 = NULL;
    uint8_t * l = NULL;
    uint8_t * l1 = NULL;

    bitmap = (uint8_t *) malloc(WIDTH * HEIGHT * sizeof(uint8_t));
    uint8_t * lut = (uint8_t *) malloc(256 * sizeof(uint8_t));
    uint16_t  i;
    //On verirife parce que l'on est pas sur que le tas soit bien dispo
    if( bitmap != NULL ) {
        //erase bitmap content

// PREMIERE METHODE
//        for(y = 0; y < HEIGHT; y++) {
//            for(x = 0; x < WIDTH; x++) {
//                bitmap[y * WIDTH + x] = 0;
//            }
//        }

// SECONDE METHODE
//        for(n = 0; n < (HEIGHT * WIDTH); n++) {
//            bitmap[n] = 0;
//        }

        //TROISIEME METHODE
        p = bitmap; //condition initiale
        n = HEIGHT * WIDTH;
        while (n != 0) {
            *p = 0; //action
            p = p+1; // action pour etape suivante
            n = n-1;
        }

        // line horizontal
        col = 0;
        p = bitmap + WIDTH * col;
        n = WIDTH;
        while (n != 0) {
            *p = 5;
            p = p + 1;
            n = n - 1;
        }

        // vertical line ( x = 4 )
        lig = 0;
        p = bitmap + lig;
        n = HEIGHT;
        while (n != 0) {
            *p = 6;
            p = p + WIDTH;
            n = n - 1;
        }

        // diag
//        p = bitmap;
//        n = HEIGHT;
//        while (n != 0) {
//            *p = 7;
//            p = p + WIDTH + 1;
//            n = n - 1;
//        }

        // ON TOUT EN UNE FOIS MAIS MOINS EFFICACE
//        n = HEIGHT * WIDTH;
//        while (n != 0) {
//            col = (WIDTH*HEIGHT - n)%WIDTH;
//            lig = (WIDTH*HEIGHT - n)/WIDTH;
//
//            if ( col == 3  || lig == 2 || col == lig) { ----> super couteux
//                *p = 5;
//            } else {
//                *p = 0;
//            }
//            p = p+1; // action pour etape suivante
//            n = n-1;
//        }


        //M Horizontal
//        n = HEIGHT/2;
//        p = bitmap;
//        p1 = bitmap + WIDTH * (HEIGHT-1);
//
//        while (n--) {
//            // ECHANGE DE LIGNE
//            l = p;
//            l1 = p1;
//            n1 = WIDTH;
//
//            while (n1--) {
//                temp = *l;
//                *l = *l1;
//                *l1 = temp;
//
//                l++;
//                l1++;
//            }
//
//            // ON PASSE AU SUIVANT
//            p = p + WIDTH;
//            p1 = p1 - WIDTH;
//        }

        //M Vertical
        n = HEIGHT;
        p = bitmap;
        p1 = bitmap + WIDTH - 1;
        while (n--) {
            //ECHANGE DE COLONNES
            l = p;
            l1 = p1;
            n1 = WIDTH / 2;
            while (n1--) {
                temp = *l;
                *l = *l1;
                *l1 = temp;

                l++;
                l--;
            }
            p +=WIDTH;
            p1 +=WIDTH;
        }

//        //en negatif
//        p = bitmap; //condition initiale
//        n = HEIGHT * WIDTH;
//        while (n != 0) {
//            *p = 255 - *p; //action
//            p = p+1; // action pour etape suivante
//            n = n-1;
//        }


        if (lut) {
            for(int i =0; i<255; i++) {
                lut[i]= (uint8_t) (255 - i);
            }

            uint8_t * p = bitmap;
            u_int32_t  counter = WIDTH * HEIGHT;
            while (counter--) {
                *p = lut[*p];
                p++;
            }
        }

        // display bitmap content
        for(y = 0; y < HEIGHT; y++) {
            for(x = 0; x < WIDTH; x++) {
                printf("0x%02x ", bitmap[y * WIDTH + x]);
            }
            printf("\n");
        }
        free(bitmap);
    } else {
        fprintf(stderr, "bitmap allocation failed\n");
    }

#ifdef WIN32
    system("pause");
#endif

    return 0;
}


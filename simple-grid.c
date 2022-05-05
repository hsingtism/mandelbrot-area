#include "mandelbrot-area.h"
#define GRID_SIZE 100
#define UPDATE_IM_SCANLINE 64

int main() {
    reseed();
    prnginit();    

    unsigned long gridTested = 0;
    const double deltaRe = 2.49 / GRID_SIZE;
    const double deltaIm = 1.15 / GRID_SIZE;

    while (1) {
        unsigned long startTime = time(NULL);
        uint64_t member = 0, notmem = 0, undeci = 0, tested = 0;
        uint32_t gRe, gIm;

        for (gIm = 0; gIm < GRID_SIZE; gIm++) {
            gRe = 0;
            if (gIm * deltaIm > 0.04) {
                uint32_t skipcount = GRID_SIZE + (uint32_t)floor(((gIm * deltaIm - 1.32) * 1.5625) / deltaRe);
                notmem += skipcount;
                tested += skipcount;
                gRe = skipcount - 1;
            }
            for (; gRe < GRID_SIZE; gRe++) {
                char memdat = membership(
                    gRe * deltaRe + _01() * deltaRe - 2.0,
                    gIm * deltaIm + _01() * deltaIm);
                member += memdat == MEMBER;
                notmem += memdat == NOT_A_MEMBER;
                undeci += memdat == UNDECIDED;
                tested++;
            }
            if (gIm % UPDATE_IM_SCANLINE == 0) {
                printf("completed %lf. %lf completed\n", gIm * deltaIm, (double)gIm / (double)GRID_SIZE);
            }
        }
        gridTested++;

        printf("GRID DONE %llu points tested; grid number %lu\n", tested, gridTested);
        printf("times:               %llu %llu\n", startTime, time(NULL));
        printf("total time:          %llu\n", time(NULL) - startTime);
        printf("total actual tested: %llu, %llu\n", tested, member + notmem + undeci, tested);
        printf("rate:                %lf\n", (double)tested / (double)(time(NULL) - startTime));
        printf("estimated area H:    %lf\n", (double)(member + undeci) / tested * 5.727);
        printf("estimated area L:    %lf\n", (double)(member         ) / tested * 5.727);
        printf("\n");

        reseed();

        if (FILE_OUTPUT == 0) continue;
        FILE *fp;
        fp = fopen("log.txt", "a");
        fprintf(fp, "%lu, %lu, %llu, %llu, %llu\n", (unsigned long)time(NULL), startTime, member, notmem, undeci);
        fclose(fp);
    }

    return 0;
}

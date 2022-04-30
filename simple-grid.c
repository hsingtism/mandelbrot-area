#include "mandelbrot-area.h"
#define GRID_SIZE 1000

int main() {
    prnginit();    
    reseed();

    unsigned long gridTested = 0;
    const double delta = 2.49 / GRID_SIZE;

    while (1) {
        unsigned long startTime = time(NULL);
        uint64_t member = 0, notmem = 0, undeci = 0, tested = 0;

        for (double wIm = 0; wIm < 1.15; wIm += delta) {
            for (double wRe = -2.0; wRe < 0.49; wRe += delta) {
                char memdat = membership(
                    wRe + _01() * delta,
                    wIm + _01() * delta);
                member += memdat == MEMBER;
                notmem += memdat == NOT_A_MEMBER;
                undeci += memdat == UNDECIDED;
                tested++;
            }
        }
        gridTested++;

        printf("GRID DONE %llu points tested; grid number %lu\n", tested, gridTested);
        printf("times:               %llu %llu\n", startTime, time(NULL));
        printf("total time:          %llu\n", time(NULL) - startTime);
        printf("total member:        %llu\n", member);
        printf("total non-member:    %llu\n", notmem);
        printf("total undecided:     %llu\n", undeci);
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
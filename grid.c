#include "mandelbrot-area.h"
#define GRID_LENGTH 256
#define MAX_DEPTH 3

void evalGrid(double startingRe, double startingIm, double delta, uint32_t* memwr, uint32_t* undeciwr) {
    uint16_t gridMem = 0, gridundeci = 0;
    for (int i = 0; i < GRID_LENGTH; i++)
        for (int r = 0; r < GRID_LENGTH; r++) {
            char memdat = membership(startingIm + delta * i, startingRe + delta * r);
            gridMem += memdat == MEMBER;
            gridundeci += memdat == UNDECIDED;
        }
    *memwr = gridMem;
    *undeciwr = gridundeci;
    return;
}

int main() {
    const unsigned long startTime = time(NULL);

    FILE *fp;
    fp = fopen("log.txt", "a");
    fprintf(fp, "instance %lu started\ntimestamp, instance, member, not member, undecided\n", startTime);
    if (FILE_OUTPUT == 0) fprintf(fp, "file output disabled.\n");
    fclose(fp);

    uint64_t member = 0, notmem = 0, undeci = 0, tested = 0;
    double adjtmem = 0, adjtnotmem = 0, adjtundeci = 0;

    double delta[MAX_DEPTH];
    for (int i = 0; i < MAX_DEPTH; i++) {
        delta[i] = 4 * 1 / pow(GRID_LENGTH, i);
    }
    double res[MAX_DEPTH];
    for (int i = 0; i < MAX_DEPTH; i++) {
        res[i] = 4 * 1 / pow(GRID_LENGTH, i * 2);
    }

    double workingPositionRe[MAX_DEPTH];    
    double workingPositionIm[MAX_DEPTH];    
    int workingDepth = 0;




    // while (1) {
    //     char memdat = membership(wre, wim);
    //     member += memdat == MEMBER;
    //     notmem += memdat == NOT_A_MEMBER;
    //     undeci += memdat == UNDECIDED;
    //     tested++;

    //     if (tested % updateInvl == 0) {
    //         printf("UPDATE AT %llu\n", tested);
    //         printf("times:               %llu %llu\n", startTime, time(NULL));
    //         printf("total time:          %llu\n", time(NULL) - startTime);
    //         printf("total actual tested: %llu, %llu\n", tested, member + notmem + undeci, tested);
    //         printf("rate:                %lf\n", (double)tested / (double)(time(NULL) - startTime));
    //         // printf("");
    //         printf("\n");

    //         if (FILE_OUTPUT == 0) continue;
    //         FILE *fp;
    //         fp = fopen("log.txt", "a");
    //         fprintf(fp, "%lu, %lu\n", (unsigned long)time(NULL), startTime);
    //         fclose(fp);
    //     }
    // }

    return 0;
}

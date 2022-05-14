#include "mandelbrot-area.h"
#define GRID_SIZE 262144
#define CHECKPOINT_INTERVAL 8192

// FOR RESTARTS ONLY, else set to 0 (gIm to -1)
#define TIMECHECKPOINT   0
#define IDCHECKPOINT     0
#define MEMBERCHECKPOINT 0
#define NOTMEMCHECKPOINT 0
#define UNDECICHECKPOINT 0
#define TESTEDCHECKPOINT 0
#define GIMCHECKPOINT    -1

int main() {
    reseed();
    prnginit();    

    unsigned long gridTested = 0;
    const double deltaRe = 2.49 / GRID_SIZE;
    const double deltaIm = 1.15 / GRID_SIZE;

    while (1) {
        unsigned long startTime = time(NULL) 
            - TIMECHECKPOINT + IDCHECKPOINT; //timestamps will not be accurate, timer itself will be close enough
        uint64_t member = MEMBERCHECKPOINT, 
                 notmem = NOTMEMCHECKPOINT, 
                 undeci = UNDECICHECKPOINT, 
                 tested = TESTEDCHECKPOINT;
        uint32_t gRe, gIm;

        for (gIm = GIMCHECKPOINT + 1; gIm < GRID_SIZE; gIm++) {
            for (gRe = 0; gRe < GRID_SIZE; gRe++) {
                char memdat = membership(
                    gRe * deltaRe + _01() * deltaRe - 2.0,
                    gIm * deltaIm + _01() * deltaIm);
                member += memdat == MEMBER;
                notmem += memdat == NOT_A_MEMBER;
                undeci += memdat == UNDECIDED;
                tested++;
            }
            if (gIm % CHECKPOINT_INTERVAL == 0) {
                printf( "TIME%lu;ID%lu;MEM%llu;NOTMEM%llu;UNDECI%llu;TESTED%llu;GIM%lu;GRE%lu\n",
                        (unsigned long)time(NULL), startTime, member, notmem, undeci, tested,
                        gIm, gRe);
                FILE *fp;
                fp = fopen("CHECKPOINT.txt", "a");
                fprintf(fp,
                        "TIME%lu;ID%lu;MEM%llu;NOTMEM%llu;UNDECI%llu;TESTED%llu;GIM%lu;GRE%lu\n",
                        (unsigned long)time(NULL), startTime, member, notmem, undeci, tested,
                        gIm, gRe);
                fclose(fp);
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

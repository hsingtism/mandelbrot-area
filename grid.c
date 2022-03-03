#include "mandelbrot-area.h"

int main() {
    uint64_t member = 0, notmem = 0, undeci = 0, tested = 0;

    const unsigned long startTime = time(NULL);

    FILE *fp;
    fp = fopen("log.txt", "a");
    fprintf(fp, "instance %lu started\ntimestamp, instance, member, not member, undecided\n", startTime);
    if (FILE_OUTPUT == 0) fprintf(fp, "file output disabled.\n");
    fclose(fp);

    double wre, wim;

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

#include "mandelbrot-area.h"

int main() {
    prnginit();
    reseed();

    uint64_t member = 0, notmem = 0, undeci = 0, tested = 0;

    const unsigned long startTime = time(NULL);

    FILE *fp;
    fp = fopen("log.txt", "a");
    fprintf(fp, "MONTE CARLO instance %lu started\ntimestamp, instance, member, not member, undecided\n", startTime);
    if (FILE_OUTPUT == 0) fprintf(fp, "file output disabled.\n");
    fclose(fp);

    while (1) {
        char memdat = membership(_22(), _22());
        member += memdat == MEMBER;
        notmem += memdat == NOT_A_MEMBER;
        undeci += memdat == UNDECIDED;
        tested++;

        if (tested % updateInvl == 0) {
            printf("UPDATE AT %llu\n", tested);
            printf("times:               %llu %llu\n", startTime, time(NULL));
            printf("total time:          %llu\n", time(NULL) - startTime);
            printf("total member:        %llu\n", member);
            printf("total non-member:    %llu\n", notmem);
            printf("total undecided:     %llu\n", undeci);
            printf("total actual tested: %llu, %llu\n", tested, member + notmem + undeci, tested);
            printf("rate:                %lf\n", (double)tested / (double)(time(NULL) - startTime));
            printf("estimated area H:    %lf\n", (double)(member + undeci) / tested * 16);
            printf("estimated area L:    %lf\n", (double)(member         ) / tested * 16);
            printf("\n");

            if(RNGSEED) reseed();

            if (FILE_OUTPUT == 0) continue;
            FILE *fp;
            fp = fopen("log.txt", "a");
            fprintf(fp, "%lu, %lu, %llu, %llu, %llu\n", (unsigned long)time(NULL), startTime, member, notmem, undeci);
            fclose(fp);
        }
    }

    return 0;
}

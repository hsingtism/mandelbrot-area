#include "mandelbrot-area.h"

#ifdef __unix__
#define RNGSEED 1
#else
#define RNGSEED 0
#endif

// from v8: https://github.com/v8/v8/blob/085fed0fb5c3b0136827b5d7c190b4bd1c23a23e/src/base/utils/random-number-generator.h#L101
uint64_t state0 = 1;
uint64_t state1 = 2;
uint64_t xorshift128plus() {
    uint64_t s1 = state0;
    uint64_t s0 = state1;
    state0 = s0;
    s1 ^= s1 << 23;
    s1 ^= s1 >> 17;
    s1 ^= s0;
    s1 ^= s0 >> 26;
    state1 = s1;
    return state0 + state1;
}

double _22(uint64_t i) {
    uint64_t u64 = 0x4010000000000001ULL | (i >> 12);
    return *(double *)&u64 - 6.0;
}

void reseed() {
    uint64_t urs0;
    uint64_t urs1;

    if (RNGSEED) {
        FILE *randsource;
        randsource = fopen("/dev/urandom", "r");

        fread(&urs0, 1, 8, randsource);
        fread(&urs1, 1, 8, randsource);
        printf("reseeded with urandom\n");
    }

    state0 ^= time(NULL) ^ urs0;
    state1 ^= time(NULL) ^ S_SEED ^ urs1;
}

int main() {
    reseed();

    printf("PRNG SEED 0: %llx\n", state0);
    printf("PRNG SEED 1: %llx\n", state1);

    for (int i = 0; i < 128; i++)
        xorshift128plus();

    uint64_t member = 0, notmem = 0, undeci = 0, tested = 0;

    const unsigned long startTime = time(NULL);

    FILE *fp;
    fp = fopen("log.txt", "a");
    fprintf(fp, "MONTE CARLO instance %lu started\ntimestamp, instance, member, not member, undecided\n", startTime);
    if (FILE_OUTPUT == 0) fprintf(fp, "file output disabled.\n");
    fclose(fp);

    while (1) {
        char memdat = membership(_22(xorshift128plus()), _22(xorshift128plus()));
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
            printf("estimated area L:    %lf\n", (double)(member - undeci) / tested * 16);
            printf("\n");

            if (FILE_OUTPUT == 0) continue;
            FILE *fp;
            fp = fopen("log.txt", "a");
            fprintf(fp, "%lu, %lu, %llu, %llu, %llu\n", (unsigned long)time(NULL), startTime, member, notmem, undeci);
            fclose(fp);
        }
    }

    return 0;
}

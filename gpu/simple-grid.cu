#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define dwellLimit 0x0000000400000000
#define C_EQUIVALENCE_THRESHOLD pow(2, -16)
#define O_EQUIVALENCE_THRESHOLD pow(2, -32)
#define S_SEED 0xa6ceb87d4389e4ef
#define FILE_OUTPUT 1

#if __unix__
#define RNGSEED 1
#else
#define RNGSEED 0
#endif

enum pointState { MEMBER,
                  NOT_A_MEMBER,
                  UNDECIDED,
                  OUT_OF_RANGE,
                  PUSH_TO_CPU };
char membership(double re, double im);

#define GRID_SIZE 262144
#define PARALLEL_SIZE 100
#define POINTS_PER_CUDA_THREAD 1000

// from v8: https://github.com/v8/v8/blob/main/src/base/utils/random-number-generator.h#L119
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

void xorshift128plusCUDA(uint64_t* state0, uint64_t* state1, uint64_t* result) {
    uint64_t s1 = *state0;
    uint64_t s0 = *state1;
    *state0 = s0;
    s1 ^= s1 << 23;
    s1 ^= s1 >> 17;
    s1 ^= s0;
    s1 ^= s0 >> 26;
    *state1 = s1;
    *result = *state0 + *state1;
}

void prnginit() {
    for (int i = 0; i < 128; i++)
        xorshift128plus();
}

// mask to [0,1)
inline double _01() {
    uint64_t u64 = 0x3FF0000000000000ULL | (xorshift128plus() >> 12);
    return *(double*)&u64 - 1.0;
}

void reseed() {
    uint64_t urs0, urs1;

    if (RNGSEED) {
        FILE* randsource;
        randsource = fopen("/dev/urandom", "r");
        fread(&urs0, 1, 8, randsource);
        fread(&urs1, 1, 8, randsource);
        fclose(randsource);
    }

    state0 ^= time(NULL) ^ urs0;
    state1 ^= time(NULL) ^ S_SEED ^ urs1;
}

void inspectPRNGstate() {
    printf("PRNG SEED 0: %llx\n", state0);
    printf("PRNG SEED 1: %llx\n", state1);
}

// extract the first bit of a double
char fneg(double x) {
    return (char)((*(uint64_t*)&x) >> 63);
}

inline char membershipt(double re, double im, uint64_t numiters) {
    if (im < -1.15 || im > 1.15 || re < -2.0 || re > 0.49 || re * re + im * im > 4.0) {
        return NOT_A_MEMBER;
    }

    // https://www.desmos.com/calculator/rmldovq5x5
    // testing main cardioid and main bulb
    if (im < 0.25 && re < -0.75 && re > -1.25) {
        const double xp1 = re + 1;
        if (fneg(xp1 * xp1 + im * im - 0.0625)) {
            return MEMBER;
        }
    } else if (re > -0.75 && im < 0.65 && re < 0.375) {
        const double adjx = re - 0.25;
        const double adjx2py2 = adjx * adjx + im * im;
        const double firstterm = adjx2py2 + 2 * 0.25 * adjx;
        if (fneg(firstterm * firstterm - 0.25 * adjx2py2)) {
            return MEMBER;
        }
    }

    const double cRe = re, cIm = im;
    double pRe, pIm, pobRe;
    double obRe = re, obIm = im;

    for (uint64_t i = 0; i < numiters; i++) {
        pRe = re;
        pIm = im;
        re = re * re - im * im + cRe;
        im = 2.0 * pRe * im + cIm;
        if (i % 5 == 1) {
            if (re * re + im * im > 4.0) return NOT_A_MEMBER;
            if (fabs(pRe - re) < C_EQUIVALENCE_THRESHOLD && fabs(pIm - im) < C_EQUIVALENCE_THRESHOLD) return MEMBER;
        }
        if (i % 2) {
            pobRe = obRe;
            obRe = obRe * obRe - obIm * obIm + cRe;
            obIm = 2 * pobRe * obIm + cIm;
            if (fabs(obRe - re) < O_EQUIVALENCE_THRESHOLD && fabs(obIm - im) < O_EQUIVALENCE_THRESHOLD) return MEMBER;
        }
    }

    return UNDECIDED;
}

__global__ void membershipKernel(uint64_t startPos, uint32_t threadLoopLength, char* resultPtr) {
    int threadId = 0; // TODO thread index
    for(int i = 0; i < threadLoopLength; i++) {

    }
}

void cudamanagement(uint64_t startPos, uint32_t threadLoopLength, uint32_t parallelSize, char* resultPtr) {
    // TODO call kernel here
}

int main(int argc, char** argv) {
    reseed();
    inspectPRNGstate();
    prnginit();
    inspectPRNGstate();

    unsigned long gridTested = 0;
    const double deltaRe = 2.49 / GRID_SIZE;
    const double deltaIm = 1.15 / GRID_SIZE;

    const uint64_t totalPointCount = GRID_SIZE * GRID_SIZE;
    uint64_t prngseed0[PARALLEL_SIZE];
    uint64_t prngseed1[PARALLEL_SIZE];
    char result[PARALLEL_SIZE * POINTS_PER_CUDA_THREAD];
    double cpuQueueRe[PARALLEL_SIZE * POINTS_PER_CUDA_THREAD];
    double cpuQueueIm[PARALLEL_SIZE * POINTS_PER_CUDA_THREAD];

    while (1) {
        unsigned long startTime = time(NULL);  // timestamps will not be accurate, timer itself will be close enough
        uint64_t member = 0,
                 notmem = 0,
                 undeci = 0,
                 tested = 0;

        for (uint64_t position = 0; position < totalPointCount; position += PARALLEL_SIZE * POINTS_PER_CUDA_THREAD) {
            reseed();
            for (uint32_t i = 0; i < PARALLEL_SIZE; i++) {
                prngseed0[i] = xorshift128plus();
                prngseed1[i] = xorshift128plus();
            }

            cudamanagement(position, POINTS_PER_CUDA_THREAD, PARALLEL_SIZE, &result[PARALLEL_SIZE * POINTS_PER_CUDA_THREAD]);

            for(uint32_t i = 0; i < PARALLEL_SIZE; i++) {
                char wv = result[i];
                member += wv == MEMBER;
                notmem += wv == NOT_A_MEMBER;
            }
        }

        if (FILE_OUTPUT == 0) continue;
        FILE* fp;
        fp = fopen("log.txt", "a");
        fprintf(fp, "%lu, %lu, %llu, %llu, %llu\n", (unsigned long)time(NULL), startTime, member, notmem, undeci);
        fclose(fp);

        gridTested++;

        printf("GRID DONE %llu points tested; grid number %lu\n", tested, gridTested);
        printf("times:               %llu %llu\n", startTime, time(NULL));
        printf("estimated area H:    %lf\n", (double)(member + undeci) / tested * 5.727);
        printf("estimated area L:    %lf\n", (double)(member) / tested * 5.727);
        printf("\n");
    }
    
    return 0;
}

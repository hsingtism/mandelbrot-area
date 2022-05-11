#include "mandelbrot-area.h"

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

void prnginit() {
    for (int i = 0; i < 128; i++)
        xorshift128plus();
}

// mask to (-2,2] 
inline double _22() {
    uint64_t u64 = 0x4010000000000001ULL | (xorshift128plus() >> 12);
    return *(double *)&u64 - 6.0;
}

// mask to [0,1)
inline double _01() {
    uint64_t u64 = 0x3FF0000000000000ULL | (xorshift128plus() >> 12);
    return *(double *)&u64 - 1.0;
}

void reseed() {
    uint64_t urs0, urs1;

    if (RNGSEED) {
        FILE *randsource;
        randsource = fopen("/dev/urandom", "r");
        fread(&urs0, 1, 8, randsource);
        fread(&urs1, 1, 8, randsource);
        fclose(randsource);
        printf("reseeded with urandom\n");
    }

    state0 ^= time(NULL) ^ urs0;
    state1 ^= time(NULL) ^ S_SEED ^ urs1;

    printf("PRNG SEED 0: %llx\n", state0);
    printf("PRNG SEED 1: %llx\n", state1);
}

inline char membership(double re, double im) {
    if (
        im < -1.15 || im > 1.15 || re < -2.0 || re > 0.49 || re * re + im * im > 4.0) {
        return NOT_A_MEMBER;
    }

    const double cRe = re, cIm = im;
    double pRe, pIm, pobRe;
    double obRe = re, obIm = im;

    for (unsigned long i = 0; i < dwellLimit; i++) {
        pRe = re;
        pIm = im;
        re = (re + im) * (re - im) + cRe;
        im = 2 * pRe * im + cIm;
        if (i % 2 || i < 5) {
            if (re * re + im * im > 4.0) return NOT_A_MEMBER;
            if (pRe == re && pIm == im) return MEMBER;
        }
        if (i % 2) {
            pobRe = obRe;
            obRe = (obRe + obIm) * (obRe - obIm) + cRe;
            obIm = 2 * pobRe * obIm + cIm;
            if (obRe == re && obIm == im) return MEMBER;
        }
    }

    printf("UNDECIDED re: %lf im: %lf - %016llx %016llx\n", cRe, cIm, *(uint64_t *)&cRe, *(uint64_t *)&cIm);
    return UNDECIDED;
}

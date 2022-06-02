#include "mandelbrot-area.h"

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
    return (char)((*(uint64_t *)&x) >> 63);
}

inline char membership(double re, double im) {
    if (   im < -1.15 || im > 1.15 
        || re < -2.0 || re > 0.49 
        || re * re + im * im > 4.0) {
        return NOT_A_MEMBER;
    }

    // https://www.desmos.com/calculator/rmldovq5x5
    // testing main cardioid and main bulb
    if (re > -1.25 && re < -0.75 && im < 0.25) {
        const double xp1 = re + 1;
        if (fneg(xp1 * xp1 + im * im - 0.0625)) {
            return MEMBER;
        }
    } else if (re > -0.75 && re < 0.375 && im < 0.65) {
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

    for (uint64_t i = 0; i < dwellLimit; i++) {
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

    printf("UNDECIDED re: %lf im: %lf - %016llx %016llx\n", cRe, cIm, *(uint64_t *)&cRe, *(uint64_t *)&cIm);
    return UNDECIDED;
}

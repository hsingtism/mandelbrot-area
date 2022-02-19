#define dwellLimit 16384
#define updateInvl 25000000

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MEMBER 0b1
#define NOT_A_MEMBER 0b10
#define UNDECIDED 0b100

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
    uint64_t u64 = 0x3FF0000000000000ULL | ((i >> 12) | 1);
    double fmanu = *(double*)&u64 - 1.0;
    return fmanu * 4 - 2; // TODO make percision full 52 bits
}

char membership(double re, double im) {
    if (re < -2.0 || re > 0.49) return NOT_A_MEMBER;
    if (im < -1.15 || im > 1.15 ) return NOT_A_MEMBER;
    if (re * re + im * im > 4.0) return NOT_A_MEMBER;

    double cRe = re, cIm = im;
    double pRe, pIm;

    // double rValues[dwellLimit];
    // double iValues[dwellLimit];

    // return bool flag (5 bits), undeci, notmem, member
    for (unsigned long i = 0; i < dwellLimit; i++) {
        // if (re * re + im * im > 4.0) return NOT_A_MEMBER;
        if (re != re || im != im) return NOT_A_MEMBER; //NaN
        if (pRe == re && pIm == im) return MEMBER; //convergence
        if (cRe == re && cIm == im) return MEMBER; // cyclic

        // rValues[i] = re;
        // iValues[i] = im;

        // for (unsigned long j = 0; j < i; j++)
        //     if (re == rValues[j] && im == iValues[j]) return MEMBER;

        pRe = re;
        pIm = im;
        // iterate
        re = (re + im) * (re - im) + cRe;
        im = 2 * pRe * im + cIm;
    }
    return UNDECIDED;
}

int main() {
    state0 = time(NULL);
    state1 = time(NULL) ^ rand();
    for (int i = 0; i < 8192; i++) 
        xorshift128plus();

    uint64_t member = 0, notmem = 0, undeci = 0, tested = 0;

    const unsigned long startTime = time(NULL);

    for (uint64_t sc = 0; sc < ULLONG_MAX; sc++) {
        char memdat = membership(_22(xorshift128plus()), _22(xorshift128plus()));
        member += memdat == MEMBER;
        notmem += memdat == NOT_A_MEMBER;
        undeci += memdat == UNDECIDED;
        tested++;

        if (sc % updateInvl == 0) {
            printf("UPDATE AT %llu\n", sc);
            printf("total time:          %llu\n", time(NULL) - startTime);
            printf("times:               %llu %llu\n", startTime, time(NULL));
            printf("rate:                %lf\n", (double)sc / (double)(time(NULL) - startTime));
            printf("total member:        %llu\n", member);
            printf("total non-member:    %llu\n", notmem);
            printf("total undecided:     %llu\n", undeci);
            printf("total actual tested: %llu, %llu\n", tested, member + notmem + undeci, tested);
            printf("estimated area H:    %lf\n", (double)(member + undeci) / tested * 16);
            printf("estimated area L:    %lf\n", (double)(member - undeci) / tested * 16);
            printf("\n");
        }
    }

    return 0;
}

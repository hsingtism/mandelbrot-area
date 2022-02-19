#define dwellLimit 1000

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

// https://stackoverflow.com/a/64699231/
double _22(uint64_t i) {
    uint64_t u64 = 0x3FF0000000000000ULL | ((i >> 12) | 1);
    double fmanu = *(double*)&u64 - 1.0;
    return (fmanu - (double)(i & 1)) * 2;
}

char membership(double re, double im) {
    // main cardoid test: (\left(x-0.25\right)^{2}+y^{2}+0.5\left(x-0.25\right))^{2}<0.25(\left(x-0.25\right)^{2}+y^{2})
    // double cX = re - 0.25;
    // if (pow((cX * cX + im * im + 0.5 * cX), 2) < 0.25 * (cX * cX + im * im)) {
    //     return MEMBER;
    // }
    // main bulb
    // if (pow((re + 0.25), 2) + im * im < 0.25) {
    //     return MEMBER;
    // }

    double cRe = re, cIm = im;
    double pRe, pIm;

    double rValues[dwellLimit];
    double iValues[dwellLimit];

    // return bool flag (5 bits), undeci, notmem, member
    for (unsigned long i = 0; i < dwellLimit; i++) {
        if (re != re || im != im) return NOT_A_MEMBER; //NaN
        // if (re * re + im * im > 4.0) return NOT_A_MEMBER;
        // if (pRe == re && pIm == im) return MEMBER;

        // rValues[i] = re;
        // iValues[i] = im;

        // for (unsigned long j = i; j >= 0; j--)
        //     if (re == rValues[j] && im == iValues[j]) return MEMBER;

        pRe = re;
        pIm = im;
        // iterate
        re = re * re - im + cRe;
        im = 2 * pRe * im + cIm;
    }
    return UNDECIDED;
}

int main() {
    // get the prng well mixed up
    for (int i = 0; i < 10; i++) 
        xorshift128plus();

    uint64_t member = 0, notmem = 0, undeci = 0, tested = 0;

    const unsigned long startTime = time(NULL);

    for (uint64_t sc = 0; sc < ULLONG_MAX; sc++) {
        char memdat = membership(_22(xorshift128plus()), _22(xorshift128plus()));
        member += memdat & MEMBER;
        notmem += memdat & NOT_A_MEMBER >> 1;
        undeci += memdat & UNDECIDED >> 2;
        tested++;

        if (sc % 16777216 == 0) {
            printf("UPDATE AT %llu\n", sc);
            printf("total time:          %llu\n", time(NULL) - startTime);
            printf("times:               %llu %llu\n", startTime, time(NULL));
            printf("rate:                %lf\n", (double)sc / (double)(time(NULL) - startTime));
            printf("total member:        %llu\n", member);
            printf("total non-member:    %llu\n", notmem);
            printf("total undecided:     %llu\n", undeci);
            printf("total actual tested: %llu, %llu\n", tested, member + notmem + undeci);
            printf("estimated area H:    %lf\n", (double)(member + undeci) / tested * 16);
            printf("estimated area L:    %lf\n", (double)(member - undeci) / tested * 16);
            printf("\n");
        }
    }

    return 0;
}

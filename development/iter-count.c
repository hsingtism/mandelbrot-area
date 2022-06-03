#include "../mandelbrot-area.h"
const uint64_t tre = 0x0000000000000000;
const uint64_t tim = 0x0000000000000000;

char fneg(double x) {
    return (char)((*(uint64_t *)&x) >> 63);
}

uint64_t mmem(double re, double im) {
    if (im < -1.15 || im > 1.15 || re < -2.0 || re > 0.49 || re * re + im * im > 4.0) {
        return 0;
    }

    if (im < 0.25 && re < -0.75 && re > -1.25) {
        const double xp1 = re + 1;
        if (fneg(xp1 * xp1 + im * im - 0.0625)) {
            return 0;
        }
    } else if (re > -0.75 && im < 0.65 && re < 0.375) {
        const double adjx = re - 0.25;
        const double adjx2py2 = adjx * adjx + im * im;
        const double firstterm = adjx2py2 + 2 * 0.25 * adjx;
        if (fneg(firstterm * firstterm - 0.25 * adjx2py2)) {
            return 0;
        }
    }

    const double cRe = re, cIm = im;
    double pRe, pIm, pobRe;
    double obRe = re, obIm = im;

    for (uint64_t i = 0;; i++) {
        pRe = re;
        pIm = im;
        re = re * re - im * im + cRe;
        im = 2.0 * pRe * im + cIm;
        if (i % 5 == 1) {
            if (re * re + im * im > 4.0) return NOT_A_MEMBER;
            if (fabs(pRe - re) < C_EQUIVALENCE_THRESHOLD && fabs(pIm - im) < C_EQUIVALENCE_THRESHOLD) return i;
        }
        if (i % 2) {
            pobRe = obRe;
            obRe = obRe * obRe - obIm * obIm + cRe;
            obIm = 2 * pobRe * obIm + cIm;
            if (fabs(obRe - re) < O_EQUIVALENCE_THRESHOLD && fabs(obIm - im) < O_EQUIVALENCE_THRESHOLD) return i;
        }
    }
}

int main() {
    printf("%llu", mmem(*(double *)&tre, *(double *)&tim));
    return 0;
}
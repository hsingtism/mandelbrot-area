#include "../mandelbrot-area.h"
const uint64_t tre = 0x0000000000000000;
const uint64_t tim = 0x0000000000000000;

uint64_t mmem(double re, double im) {
    if (im < -1.15 || im > 1.15 || re < -2.0 || re > 0.49 || re * re + im * im > 4.0) {
        return 0;
    }

    const double cRe = re, cIm = im;
    double pRe, pIm, pobRe;
    double obRe = re, obIm = im;

    for (uint64_t i = 0;; i++) {
        pRe = re;
        pIm = im;
        re = (re + im) * (re - im) + cRe;
        im = 2 * pRe * im + cIm;
        if (i % 2 || i < 5) {
            if (re * re + im * im > 4.0) return i;
            if (fabs(pRe - re) < C_EQUIVALENCE_THRESHOLD && fabs(pIm - im) < C_EQUIVALENCE_THRESHOLD) return i;
        }
        if (i % 2) {
            pobRe = obRe;
            obRe = (obRe + obIm) * (obRe - obIm) + cRe;
            obIm = 2 * pobRe * obIm + cIm;
            if (fabs(obRe - re) < O_EQUIVALENCE_THRESHOLD && fabs(obIm - im) < O_EQUIVALENCE_THRESHOLD) return i;
        }
    }
}

int main() {
    printf("%llu", mmem(*(double *)&tre, *(double *)&tim));
    return 0;
}
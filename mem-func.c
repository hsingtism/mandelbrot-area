#include "mandelbrot-area.h"

char membership(double re, double im) {
    if (re < -2.0 || re > 0.49) return NOT_A_MEMBER;
    if (im < -1.15 || im > 1.15) return NOT_A_MEMBER;
    if (re * re + im * im > 4.0) return NOT_A_MEMBER;

    const double cRe = re, cIm = im;
    double inRe, inIm, pRe, pIm, pobRe;
    double obRe = re, obIm = im;

    for (unsigned long i = 0; i < dwellLimit; i++) {
        if (re * re + im * im > 4.0) return NOT_A_MEMBER;
        if (re != re || im != im) return NOT_A_MEMBER;  // NaN
        if (pRe == re && pIm == im) return MEMBER;      // convergence

        pRe = re;
        pIm = im;
        // iterate
        re = (re + im) * (re - im) + cRe;
        im = 2 * pRe * im + cIm;

        if (cRe == re && cIm == im) return MEMBER;  // cyclic

        if (i % 2) {  // TODO optimize this
            pobRe = obRe;
            obRe = (obRe + obIm) * (obRe - obIm) + cRe;
            obIm = 2 * pobRe * obIm + cIm;
            if (obRe == re && obIm == im) return MEMBER;
        }
    }
    printf("UNDECIDED re: %lf im: %lf - %016llx %016llx\n", cRe, cIm, *(uint64_t *)&cRe, *(uint64_t *)&cIm);

    if (FILE_OUTPUT == 1) {
        FILE *fp;
        fp = fopen("log.txt", "a");
        fprintf(fp, "UNDECIDED re: %lf im: %lf - %016llx %016llx\n", cRe, cIm, *(uint64_t *)&cRe, *(uint64_t *)&cIm);
        fclose(fp);
    }

    return UNDECIDED;
}
// MODIFY SETTINGS HERE
#define dwellLimit pow(2, 32)
#define EQUIVALENCE_THRESHOLD pow(2, -16)
#define S_SEED 0xa6ceb87d4389e4ef
#define FILE_OUTPUT 1

#ifdef __unix__
#define RNGSEED 1
#else
#define RNGSEED 0
#endif
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MEMBER 0b1
#define NOT_A_MEMBER 0b10
#define UNDECIDED 0b100
char membership(double re, double im);
double _22();
double _01();
void reseed();
void prnginit();

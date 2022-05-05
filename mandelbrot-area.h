// MODIFY SETTINGS HERE
#define dwellLimit 4294967296
#define S_SEED 0xade5323c6e74d2fe
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

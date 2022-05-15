// MODIFY SETTINGS HERE
#define dwellLimit 0x0000000400000000
#define C_EQUIVALENCE_THRESHOLD pow(2, -16)
#define O_EQUIVALENCE_THRESHOLD pow(2, -32)
#define S_SEED 0xa6ceb87d4389e4ef
#define FILE_OUTPUT 1

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef __unix__
    #define RNGSEED 1
#else
    #define RNGSEED 0
#endif
void reseed();
void inspectPRNGstate(); 
void prnginit();
double _22();
double _01();

#define MEMBER 0b1
#define NOT_A_MEMBER 0b10
#define UNDECIDED 0b100
char membership(double re, double im);

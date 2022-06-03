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

enum pointState{MEMBER, NOT_A_MEMBER, UNDECIDED};
char membership(double re, double im);

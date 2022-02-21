// MODIFY SETTINGS HERE
#define dwellLimit 1073741824
#define updateInvl 67108864
#define S_SEED 0xade5323c6e74d2fe
#define FILE_OUTPUT 1

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MEMBER 0b1
#define NOT_A_MEMBER 0b10
#define UNDECIDED 0b100
char membership(double re, double im);
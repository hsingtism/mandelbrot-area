# A program that approximates the area of the Mandelbrot set
## This is a program that approximates the area of the Mandelbrot set using a purely Monte Carlo method.
## Starting parameters (the `#define`s)
change before compilation
- `dwellLimit` - `unsigned long` - the maximum number of times a number will be iterated and checked for divergence, convergence, and cyclic orbits before it is deemed as `UNDECIDED`
- `updateInvl` - `unsigned long long` - the number of values that will be checked until an update message is printed
- `S_SEED` - `unsigned long long` - this number will be xor'ed with the system time to generate a seed
## Notable things
- The random coordinates are generated the `xorshift128plus` PRNG function seeded using system time *in seconds* and `rand()`. The same function that V8 (JS engine) uses.
- Since the program is random, multiple instances can be ran independently in parallel and the results can be averaged together (should be weighted). When doing this, **ensure that the PRNG receive different initial seeds**
- `<stdbool.h>` is not used
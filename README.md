# A program that approximates the area of the Mandelbrot set
## This is a program that approximates the area of the Mandelbrot set using a purely Monte Carlo method.
## Starting parameters (the `#define`s)
change before compilation
- `dwellLimit` - `unsigned long` - the maximum number of times a number will be iterated and checked for divergence, convergence, and cyclic orbits before it is deemed as `UNDECIDED`
- `updateInvl` - `unsigned long long` - the number of values that will be checked until an update message is printed
- `S_SEED` - `unsigned long long` - this number will be xor'ed with the system time to generate a seed
- `FILE_OUTPUT` - `int` when used, actually boolean - `1` to enable **logging** to file, `0` to disable. 
## Output
Output is printed to console, it is also written into `log.txt`. Multiple instances of the program (when ran in the same directory) will write to the same file, each instance is identified by its start time provided by `time.h`

Output is formatted as follows
- When an instance is started - `instance 1645383166 started` is printed (or whatever the time is) followed by a row of keys
- When an update is printed (see `updateInvl`), a row of comma delimited data is printed in the format of `timestamp, instance, member, not member, undecided`; the number of computed area is simply: 16 * (member $\pm$ undecided) / (member + undecided + not member) 
- When membership of a value cannot be determined (dwell limit reached), the number and its exact floating point bit representation is printed 
## Notable things
- The random coordinates are generated the `xorshift128plus` PRNG function seeded using system time *in seconds* and `rand()`. The same function that V8 (JS engine) uses.
- Since the program is random, multiple instances can be ran independently in parallel and the results can be averaged together (should be weighted). When doing this, **ensure that the PRNG receive different initial seeds**
- `<stdbool.h>` is not used
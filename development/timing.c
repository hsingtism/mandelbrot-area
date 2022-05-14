#include "../mandelbrot-area.h"

int main() {
    clock_t begin = clock();

    prnginit();
    uint64_t sum = 0;
    for (uint32_t i = 0; i < 5000000; i++) {
        sum += membership(
            _01() * 2.49 - 2.0,
            _01() * 1.15);
    }

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("%lf\n", time_spent);
    printf("%llu\n", sum);
}

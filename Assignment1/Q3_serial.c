#include <stdio.h>
#include <omp.h>

int main() {
    long iterations = 100000000;
    double delta = 1.0 / iterations;
    double total_sum = 0.0;

    double t_start = omp_get_wtime();

    for (long i = 0; i < iterations; i++) {
        double midpoint = (i + 0.5) * delta;
        total_sum += 4.0 / (1.0 + midpoint * midpoint);
    }

    double pi_result = delta * total_sum;
    double t_end = omp_get_wtime();

    printf("Calculated Pi: %.10f\n", pi_result);
    printf("Serial Duration: %f s\n", t_end - t_start);

    return 0;
}
#include <stdio.h>
#include <omp.h>

int main() {
    long total_steps = 100000000;
    double delta = 1.0 / total_steps;
    double combined_sum = 0.0;

    double t_start = omp_get_wtime();

    #pragma omp parallel for reduction(+:combined_sum)
    for (long i = 0; i < total_steps; i++) {
        double pos = (i + 0.5) * delta;
        combined_sum += 4.0 / (1.0 + pos * pos);
    }

    double final_pi = delta * combined_sum;
    double t_end = omp_get_wtime();

    printf("Pi: %.10f\n", final_pi);
    printf("Parallel Time: %f s\n", t_end - t_start);

    return 0;
}
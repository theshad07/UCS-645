#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 65536

int main() {
    double *vec_x = malloc(N * sizeof(double));
    double *vec_y = malloc(N * sizeof(double));
    double val_a = 2.5;

    for (size_t i = 0; i < N; i++) {
        vec_x[i] = i * 1.0;
        vec_y[i] = i * 2.0;
    }

    double t1 = omp_get_wtime();

    #pragma omp parallel for
    for (size_t i = 0; i < N; i++) {
        vec_x[i] = val_a * vec_x[i] + vec_y[i];
    }

    double t2 = omp_get_wtime();

    printf("Threads: %d | Time: %f\n", omp_get_max_threads(), t2 - t1);

    free(vec_x);
    free(vec_y);

    return 0;
}
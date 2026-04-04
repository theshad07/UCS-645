#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void simulate_heat_2d(int size, int iterations) {
    double **u = malloc(size * sizeof(double *));
    double **u_new = malloc(size * sizeof(double *));
    for (int i = 0; i < size; i++) {
        u[i] = calloc(size, sizeof(double));
        u_new[i] = calloc(size, sizeof(double));
    }

    for (int i = 0; i < size; i++) u[0][i] = 100.0; 

    double start = omp_get_wtime();

    for (int iter = 0; iter < iterations; iter++) {
        double total_heat = 0.0;

        #pragma omp parallel for collapse(2) schedule(static) reduction(+:total_heat)
        for (int i = 1; i < size - 1; i++) {
            for (int j = 1; j < size - 1; j++) {
                u_new[i][j] = 0.25 * (u[i-1][j] + u[i+1][j] + u[i][j-1] + u[i][j+1]);
                total_heat += u_new[i][j];
            }
        }

        #pragma omp parallel for collapse(2)
        for (int i = 1; i < size - 1; i++) {
            for (int j = 1; j < size - 1; j++) {
                u[i][j] = u_new[i][j];
            }
        }
        if (iter % 100 == 0) printf("Iteration %d: Heat sum = %.2f\n", iter, total_heat);
    }

    double end = omp_get_wtime();
    printf("Heat Diffusion Time: %f seconds\n", end - start);

    for (int i = 0; i < size; i++) { free(u[i]); free(u_new[i]); }
    free(u); free(u_new);
}

int main() {
    simulate_heat_2d(500, 1000); // 500x500 grid, 1000 steps
    return 0;
}
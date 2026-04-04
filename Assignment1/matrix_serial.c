#include <stdio.h>
#include <omp.h>

#define MAX_DIM 500

int main() {
    static double gridA[MAX_DIM][MAX_DIM];
    static double gridB[MAX_DIM][MAX_DIM];
    static double gridC[MAX_DIM][MAX_DIM];

    for (int r = 0; r < MAX_DIM; r++) {
        for (int c = 0; c < MAX_DIM; c++) {
            gridA[r][c] = 1.0;
            gridB[r][c] = 2.0;
            gridC[r][c] = 0.0;
        }
    }

    double t_start = omp_get_wtime();

    for (int r = 0; r < MAX_DIM; r++) {
        for (int c = 0; c < MAX_DIM; c++) {
            for (int k = 0; k < MAX_DIM; k++) {
                gridC[r][c] += gridA[r][k] * gridB[k][c];
            }
        }
    }

    double t_end = omp_get_wtime();

    printf("Serial Execution: %f seconds\n", t_end - t_start);
    printf("Result check: %f\n", gridC[0][0]);

    return 0;
}
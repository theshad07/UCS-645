#include <stdio.h>
#include <omp.h>

#define SIZE 500

int main() {
    static double mat_a[SIZE][SIZE], mat_b[SIZE][SIZE], mat_c[SIZE][SIZE];

    for (int r = 0; r < SIZE; r++) {
        for (int c = 0; c < SIZE; c++) {
            mat_a[r][c] = 1.0;
            mat_b[r][c] = 2.0;
            mat_c[r][c] = 0.0;
        }
    }

    double t1 = omp_get_wtime();

    #pragma omp parallel for collapse(2)
    for (int r = 0; r < SIZE; r++) {
        for (int c = 0; c < SIZE; c++) {
            for (int k = 0; k < SIZE; k++) {
                mat_c[r][c] += mat_a[r][k] * mat_b[k][c];
            }
        }
    }

    double t2 = omp_get_wtime();

    printf("2D Parallel Result: %f s\n", t2 - t1);
    printf("Check: %f\n", mat_c[0][0]);

    return 0;
}
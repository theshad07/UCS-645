#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define SIZE 1000

int main() {
    double (*A)[SIZE] = malloc(sizeof(double[SIZE][SIZE]));
    double (*B)[SIZE] = malloc(sizeof(double[SIZE][SIZE]));
    double (*C)[SIZE] = malloc(sizeof(double[SIZE][SIZE]));

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            A[i][j] = 1.0;
            B[i][j] = 2.0;
            C[i][j] = 0.0;
        }
    }

    double start = omp_get_wtime();

    #pragma omp parallel for
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            for (int k = 0; k < SIZE; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    double end = omp_get_wtime();

    printf("1D Parallel Completion: %f seconds\n", end - start);
    printf("Sample Result: C[0][0] = %.2f\n", C[0][0]);

    free(A); free(B); free(C);
    return 0;
}
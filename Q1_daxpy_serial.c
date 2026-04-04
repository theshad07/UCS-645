#include <stdio.h>
#include <stdlib.h>
#include <omp.h>


#define VECTOR_SIZE (1 << 16) 

int main() {
   
    double *vecX = malloc(VECTOR_SIZE * sizeof(double));
    double *vecY = malloc(VECTOR_SIZE * sizeof(double));
    double scalar_a = 2.5;

    
    #pragma omp parallel for
    for (int i = 0; i < VECTOR_SIZE; i++) {
        vecX[i] = i * 1.1; 
        vecY[i] = i * 2.2;
    }

    printf("Starting DAXPY computation on %d elements...\n", VECTOR_SIZE);
    
    double timestamp_start = omp_get_wtime();

    
    #pragma omp parallel for
    for (int i = 0; i < VECTOR_SIZE; i++) {
        vecX[i] = scalar_a * vecX[i] + vecY[i];
    }

    double timestamp_end = omp_get_wtime();

   
    printf("Total Compute Time: %f seconds\n", timestamp_end - timestamp_start);
    printf("Sample Result: vecX[100] = %f\n", vecX[100]);

  
    free(vecX);
    free(vecY);
    
    return 0;
}
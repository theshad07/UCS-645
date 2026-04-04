#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#define MATCH 2
#define MISMATCH -1
#define GAP -1

int max_val(int a, int b) { return (a > b) ? a : b; }

void smith_waterman_parallel(const char* s1, const char* s2) {
    int n1 = strlen(s1);
    int n2 = strlen(s2);
    int max_score = 0;

    int** H = (int**)malloc((n1 + 1) * sizeof(int*));
    for (int i = 0; i <= n1; i++) H[i] = (int*)calloc((n2 + 1), sizeof(int));

    double start = omp_get_wtime();

    for (int k = 1; k <= (n1 + n2 - 1); k++) {
        #pragma omp parallel for schedule(static) reduction(max:max_score)
        for (int i = 1; i <= n1; i++) {
            int j = k - i + 1;
            if (j >= 1 && j <= n2) {
                int diag = H[i-1][j-1] + (s1[i-1] == s2[j-1] ? MATCH : MISMATCH);
                int up = H[i-1][j] + GAP;
                int left = H[i][j-1] + GAP;

                H[i][j] = max_val(0, max_val(diag, max_val(up, left)));
                if (H[i][j] > max_score) max_score = H[i][j];
            }
        }
    }

    double end = omp_get_wtime();
    printf("Max Alignment Score: %d\n", max_score);
    printf("Sequence Alignment Time: %f seconds\n", end - start);

    for (int i = 0; i <= n1; i++) free(H[i]);
    free(H);
}

int main() {
    // Sample DNA sequences
    const char* seq1 = "ATCGATCGATCGATCGATCGATCGATCGATCG"; 
    const char* seq2 = "ATCGGTCGATCGATCGATCGATCGATCGATCG";
    
    smith_waterman_parallel(seq1, seq2);
    return 0;
}
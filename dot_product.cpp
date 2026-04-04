#include <mpi.h>
#include <iostream>
#include <vector>
#include <numeric>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int N = 1000; 
    int local_size = N / size;

    std::vector<int> A, B;
    std::vector<int> local_A(local_size), local_B(local_size);
    int local_dot = 0, global_dot = 0;

    if (rank == 0) {
        A.resize(N);
        B.resize(N);
        for (int i = 0; i < N; i++) {
            A[i] = i + 1;
            B[i] = N - i;
        }
    }

    MPI_Scatter(A.data(), local_size, MPI_INT, local_A.data(), local_size, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(B.data(), local_size, MPI_INT, local_B.data(), local_size, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    double start = MPI_Wtime();

    for (int i = 0; i < local_size; i++) {
        local_dot += local_A[i] * local_B[i];
    }

    MPI_Reduce(&local_dot, &global_dot, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    double end = MPI_Wtime();

    if (rank == 0) {
        std::cout << "========================================" << std::endl;
        std::cout << "Processes:      " << size << std::endl;
        std::cout << "Dot Product:    " << global_dot << std::endl;
        std::cout << "Execution Time: " << (end - start) << " seconds" << std::endl;
        std::cout << "========================================" << std::endl;
    }

    MPI_Finalize();
    return 0;
}
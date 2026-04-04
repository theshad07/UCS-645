#include <mpi.h>
#include <iostream>
#include <vector>

#define TOTAL_N 500000000

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double multiplier;

    if (rank == 0) {
        std::cout << "Enter scaling multiplier: ";
        std::cin >> multiplier;
    }

    MPI_Bcast(&multiplier, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    long long local_n = TOTAL_N / size;

    std::vector<double> A(local_n);
    std::vector<double> B(local_n);

    for (long long i = 0; i < local_n; i++) {
        A[i] = 1.0;
        B[i] = 2.0 * multiplier;
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double start = MPI_Wtime();

    double local_sum = 0.0;
    for (long long i = 0; i < local_n; i++) {
        local_sum += A[i] * B[i];
    }

    double global_sum = 0.0;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE,
               MPI_SUM, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    double end = MPI_Wtime();

    if (rank == 0) {
        std::cout << "Final Dot Product = " << global_sum << std::endl;
        std::cout << "Execution Time: " << (end - start) << " seconds" << std::endl;
    }

    MPI_Finalize();
    return 0;
}
#include <mpi.h>
#include <iostream>
#include <vector>

#define N 65536

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double a = 2.5;
    int local_size = N / size;

    std::vector<double> X(N), Y(N);
    std::vector<double> local_X(local_size);
    std::vector<double> local_Y(local_size);

    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            X[i] = i * 1.0;
            Y[i] = i * 2.0;
        }
    }

    MPI_Scatter(X.data(), local_size, MPI_DOUBLE,
                local_X.data(), local_size, MPI_DOUBLE,
                0, MPI_COMM_WORLD);

    MPI_Scatter(Y.data(), local_size, MPI_DOUBLE,
                local_Y.data(), local_size, MPI_DOUBLE,
                0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    double start = MPI_Wtime();

    for (int i = 0; i < local_size; i++) {
        local_X[i] = a * local_X[i] + local_Y[i];
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double end = MPI_Wtime();

    MPI_Gather(local_X.data(), local_size, MPI_DOUBLE,
               X.data(), local_size, MPI_DOUBLE,
               0, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "Execution Time: " << (end - start) << " seconds" << std::endl;
    }

    MPI_Finalize();
    return 0;
}
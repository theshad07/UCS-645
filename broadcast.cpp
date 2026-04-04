#include <mpi.h>
#include <iostream>
#include <vector>

#define N 10000000

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::vector<double> data(N);

    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            data[i] = i * 1.0;
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double start_A = MPI_Wtime();

    if (rank == 0) {
        for (int i = 1; i < size; i++) {
            MPI_Send(data.data(), N, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
        }
    } else {
        MPI_Recv(data.data(), N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double end_A = MPI_Wtime();

    if (rank == 0) {
        std::cout << "Manual Broadcast Time: " << (end_A - start_A) << " seconds" << std::endl;
        
        for (int i = 0; i < N; i++) {
            data[i] = i * 2.0;
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);

    double start_B = MPI_Wtime();

    MPI_Bcast(data.data(), N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    double end_B = MPI_Wtime();

    if (rank == 0) {
        std::cout << "MPI_Bcast Time: " << (end_B - start_B) << " seconds" << std::endl;
    }

    MPI_Finalize();
    return 0;
}
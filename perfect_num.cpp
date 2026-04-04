#include <mpi.h>
#include <iostream>

bool isPerfect(int n) {
    if (n < 2) return false;

    int sum = 1;
    for (int i = 2; i <= n / 2; i++) {
        if (n % i == 0)
            sum += i;
    }

    return (sum == n);
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int max_value;

    if (rank == 0) {
        std::cout << "Enter maximum value: ";
        std::cin >> max_value;
    }

    MPI_Bcast(&max_value, 1, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    double start = MPI_Wtime();

    if (rank == 0) {
        int current = 2;
        int active_workers = size - 1;

        while (active_workers > 0) {
            int msg;
            MPI_Status status;

            MPI_Recv(&msg, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);

            int worker = status.MPI_SOURCE;

            if (msg > 0) {
                std::cout << "Perfect Number: " << msg << std::endl;
            }

            if (current <= max_value) {
                MPI_Send(&current, 1, MPI_INT, worker, 0, MPI_COMM_WORLD);
                current++;
            } else {
                int stop = 0;
                MPI_Send(&stop, 1, MPI_INT, worker, 0, MPI_COMM_WORLD);
                active_workers--;
            }
        }
    } else {
        int number;
        int request = 0;
        
        MPI_Send(&request, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

        while (true) {
            MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            if (number == 0) break;

            int result = isPerfect(number) ? number : -number;

            MPI_Send(&result, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double end = MPI_Wtime();

    if (rank == 0) {
        std::cout << "Execution Time: " << (end - start) << " seconds\n";
    }

    MPI_Finalize();
    return 0;
}
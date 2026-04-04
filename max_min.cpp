#include <mpi.h>
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int N = 1000;
    std::vector<int> numbers(N);

    std::mt19937 prng(std::random_device{}() + rank);
    std::uniform_int_distribution<int> dist(0, 10000);

    for (auto& n : numbers) {
        n = dist(prng);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double start = MPI_Wtime();

    auto [min_it, max_it] = std::minmax_element(numbers.begin(), numbers.end());
    
    struct {
        int value;
        int rank;
    } local_maxloc, global_maxloc, local_minloc, global_minloc;

    local_maxloc = {*max_it, rank};
    local_minloc = {*min_it, rank};

    MPI_Reduce(&local_maxloc, &global_maxloc, 1, MPI_2INT, MPI_MAXLOC, 0, MPI_COMM_WORLD);
    MPI_Reduce(&local_minloc, &global_minloc, 1, MPI_2INT, MPI_MINLOC, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    double end = MPI_Wtime();

    if (rank == 0) {
        std::cout << "========================================" << std::endl;
        std::cout << "Processes:  " << size << std::endl;
        std::cout << "Global Max: " << global_maxloc.value << " (Process " << global_maxloc.rank << ")" << std::endl;
        std::cout << "Global Min: " << global_minloc.value << " (Process " << global_minloc.rank << ")" << std::endl;
        std::cout << "Time:       " << (end - start) << " seconds" << std::endl;
        std::cout << "========================================" << std::endl;
    }

    MPI_Finalize();
    return 0;
}
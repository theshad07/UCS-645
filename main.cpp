#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>
#include <omp.h>


void correlate(int ny, int nx, const float* data, float* result);

int main(int argc, char* argv[]) {
  
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <rows_ny> <cols_nx>\n";
        return EXIT_FAILURE;
    }

    const int ny = std::stoi(argv[1]);
    const int nx = std::stoi(argv[2]);

    if (ny <= 0 || nx <= 0) {
        std::cerr << "Error: Dimensions must be positive integers.\n";
        return EXIT_FAILURE;
    }

    // 2. Memory Allocation
    
    std::vector<float> data(static_cast<size_t>(ny) * nx);
    std::vector<float> result(static_cast<size_t>(ny) * ny, 0.0f);

    // 3. Modern Random Number Generation
   
    std::mt19937 prng(std::random_device{}()); 
    std::uniform_real_distribution<float> dist(0.0f, 100.0f);

    for (auto& val : data) {
        val = dist(prng);
    }

    std::cout << "Computing correlation for " << ny << "x" << nx << " matrix...\n";
    std::cout << "Threads available: " << omp_get_max_threads() << "\n";

    // 4. Precise Timing
    
    auto start = std::chrono::high_resolution_clock::now();

    correlate(ny, nx, data.data(), result.data());

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;

    // 5. Clean Output Formatting
    std::cout << "------------------------------------------\n";
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Execution Time: " << diff.count() << " seconds\n";
    std::cout << "------------------------------------------\n";

   
    if (ny > 0) {
        std::cout << "Top-left correlation (result[0]): " << result[0] << "\n";
    }

    return EXIT_SUCCESS;
}
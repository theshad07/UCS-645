#include <cmath>
#include <vector>
#include <omp.h>
#include <iostream>

#define USE_OPENMP 1


void correlate(int ny, int nx, const float* data, float* result) {
   
    std::vector<double> mean(ny, 0.0);
    std::vector<double> inv_stddev(ny, 0.0);

    // Steps 1 & 2: Combined pass for better cache reuse
    #if USE_OPENMP
    #pragma omp parallel for schedule(static)
    #endif
    for (int i = 0; i < ny; i++) {
        double sum = 0.0;
        double sq_sum = 0.0;
        const float* row = &data[i * nx];

       
        #pragma omp simd reduction(+:sum, sq_sum)
        for (int x = 0; x < nx; x++) {
            float val = row[x];
            sum += val;
            sq_sum += (double)val * val;
        }

        double m = sum / nx;
        mean[i] = m;
        
 
        double variance = sq_sum - (double)sum * sum / nx;
        inv_stddev[i] = (variance > 0) ? 1.0 / std::sqrt(variance) : 0.0;
    }

    // Step 3: Correlation Matrix
    
    #if USE_OPENMP
    #pragma omp parallel for schedule(dynamic, 1) 
    #endif
    for (int i = 0; i < ny; i++) {
        for (int j = 0; j <= i; j++) {
            double dot_product = 0.0;
            const float* row_i = &data[i * nx];
            const float* row_j = &data[j * nx];
            double m_i = mean[i];
            double m_j = mean[j];

           
            #pragma omp simd reduction(+:dot_product)
            for (int x = 0; x < nx; x++) {
                dot_product += (double)(row_i[x] - m_i) * (row_j[x] - m_j);
            }

            
            float res = (float)(dot_product * inv_stddev[i] * inv_stddev[j]);
            
          
            result[i + j * ny] = res;
            result[j + i * ny] = res; 
        }
    }
}
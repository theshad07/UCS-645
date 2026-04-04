#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

typedef struct {
    double x, y, z;
    double fx, fy, fz;
} Particle;

void compute_forces(Particle* atoms, int n, double* energy_total) {
    double energy_sum = 0.0;
    double start = omp_get_wtime();

    #pragma omp parallel for schedule(guided) reduction(+:energy_sum)
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            double dx = atoms[i].x - atoms[j].x;
            double dy = atoms[i].y - atoms[j].y;
            double dz = atoms[i].z - atoms[j].z;
            double r2 = dx*dx + dy*dy + dz*dz;

            double r6inv = 1.0 / (r2 * r2 * r2);
            double force_mag = 24.0 * r6inv * (2.0 * r6inv - 1.0) / r2;

            energy_sum += 4.0 * r6inv * (r6inv - 1.0);

            #pragma omp atomic
            atoms[i].fx += dx * force_mag;
            #pragma omp atomic
            atoms[i].fy += dy * force_mag;
            #pragma omp atomic
            atoms[i].fz += dz * force_mag;

            #pragma omp atomic
            atoms[j].fx -= dx * force_mag;
            #pragma omp atomic
            atoms[j].fy -= dy * force_mag;
            #pragma omp atomic
            atoms[j].fz -= dz * force_mag;
        }
    }

    *energy_total = energy_sum;
    double end = omp_get_wtime();
    printf("Compute Time: %f seconds\n", end - start);
}

int main() {
    int N = 5000; // Problem size
    double total_energy = 0;
    
    // Allocate memory for particles [cite: 209, 212, 214]
    Particle* atoms = (Particle*)malloc(N * sizeof(Particle));
    
    // Initialize particles with random positions
    for(int i = 0; i < N; i++) {
        atoms[i].x = (double)rand() / RAND_MAX;
        atoms[i].y = (double)rand() / RAND_MAX;
        atoms[i].z = (double)rand() / RAND_MAX;
        atoms[i].fx = atoms[i].fy = atoms[i].fz = 0;
    }

    // Call the function you wrote [cite: 957]
    compute_forces(atoms, N, &total_energy);

    printf("Final Potential Energy: %f\n", total_energy);

    free(atoms);
    return 0;
}
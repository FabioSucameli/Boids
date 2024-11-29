
## Parallel Implementation

- **Parallel Processing**: OpenMP is used to divide computational tasks across multiple threads, significantly reducing execution time for large numbers of boids.
- **Performance Measurement**: The implementation uses `omp_get_wtime()` for precise timing of execution across different thread configurations.

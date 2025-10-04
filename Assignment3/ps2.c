#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void matrix_add(int size, int threads) 
{
    int i, j;
    double **a = malloc(size * sizeof(double *));
    double **b = malloc(size * sizeof(double *));
    double **c = malloc(size * sizeof(double *));

    for (i = 0; i < size; i++) 
    {
        a[i] = malloc(size * sizeof(double));
        b[i] = malloc(size * sizeof(double));
        c[i] = malloc(size * sizeof(double));
        for (j = 0; j < size; j++) 
        {
            a[i][j] = i + j;
            b[i][j] = i - j;
        }
    }

    omp_set_num_threads(threads);

    double start = omp_get_wtime();
    #pragma omp parallel for collapse(2)
    for (i = 0; i < size; i++) 
    {
        for (j = 0; j < size; j++) 
        {
            c[i][j] = a[i][j] + b[i][j];
        }
    }
    double end = omp_get_wtime();

    printf("Size: %d, Threads: %d, Time: %f seconds\n", size, threads, end - start);

    for (i = 0; i < size; i++) 
    {
        free(a[i]);
        free(b[i]);
        free(c[i]);
    }
    free(a);
    free(b);
    free(c);
}

int main() {
    int sizes[] = {250, 500, 750, 1000, 2000};
    int thread_counts[] = {1, 2, 4, 8};

    for (int s = 0; s < 5; s++) {
        double serial_time = 0.0;

        int size = sizes[s];
        omp_set_num_threads(1);
        double start = omp_get_wtime();
        matrix_add(size, 1);
        double end = omp_get_wtime();
        serial_time = end - start;

        for (int t = 1; t < 4; t++) {
            int threads = thread_counts[t];
            start = omp_get_wtime();
            matrix_add(size, threads);
            end = omp_get_wtime();
            double parallel_time = end - start;
            double speedup = serial_time / parallel_time;
            printf("Size: %d, Threads: %d, Speedup: %.2f\n", size, threads, speedup);
        }
        printf("\n");
    }
    return 0;
}

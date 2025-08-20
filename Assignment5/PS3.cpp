#include <stdio.h>
#include <stdlib.h>
#include <omp.h>


int main() {
    int N = 500;
    int A[N][N], x[N], y[N];

    for (int i = 0; i < N; i++) {
        x[i] = rand() % 10;
        for (int j = 0; j < N; j++)
            A[i][j] = rand() % 10;
    }

    double st = omp_get_wtime();

    #pragma omp parallel for private(i, j) shared(A, x, y)
    for (int i = 0; i < N; i++) {
        y[i] = 0;
        for (int j = 0; j < N; j++)
            y[i] += A[i][j] * x[j];
    }

    double end = omp_get_wtime();
    printf("Matrix-Vector Multiplication completed in %f seconds\n", end - st);

    return 0;
}

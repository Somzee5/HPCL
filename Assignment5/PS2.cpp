#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define R 500
#define C 500

int main() {
    static int A[R][C];
    int scalar = 5;

    for (int i = 0; i < R; i++)
        for (int j = 0; j < C; j++)
            A[i][j] = rand() % 10;

    double st = omp_get_wtime();

    #pragma omp parallel for collapse(2) shared(A, scalar)
    for (int i = 0; i < R; i++)
        for (int j = 0; j < C; j++)
            A[i][j] *= scalar;

    double end = omp_get_wtime();
    printf("Matrix-Scalar Multiplication completed in %f seconds\n", end - st);

    return 0;
}

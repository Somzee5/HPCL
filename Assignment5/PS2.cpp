#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    int r = 500;
    int c = 500;
    int A[r][c];
    int scalar = 5;

    for (int i = 0; i < r; i++)
        for (int j = 0; j < c; j++)
            A[i][j] = rand() % 10;

    double st = omp_get_wtime();

    #pragma omp parallel for private(i, j) shared(A, scalar)
    for (int i = 0; i < r; i++)
        for (int j = 0; j < c; j++)
            A[i][j] *= scalar;

    double end = omp_get_wtime();
    printf("Matrix-Scalar Multiplication completed in %f seconds\n", end - st);

    return 0;
}

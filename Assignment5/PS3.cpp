#include <iostream>
#include <cstdlib>
#include <omp.h>
using namespace std;

#define N 500

int main() {
    static int A[N][N], x[N], y[N];
    int i, j;   

    for (i = 0; i < N; i++) {
        x[i] = rand() % 10;
        for (j = 0; j < N; j++)
            A[i][j] = rand() % 10;
    }

    double st = omp_get_wtime();

    #pragma omp parallel for private(j) shared(A, x, y)
    for (i = 0; i < N; i++) {
        y[i] = 0;
        for (j = 0; j < N; j++)
            y[i] += A[i][j] * x[j];
    }

    double end = omp_get_wtime();
    cout << "Matrix-Vector Multiplication completed in "
         << (end - st) << " seconds\n";

    return 0;
}

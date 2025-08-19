#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    int i;
    int n;

    printf("Enter Size: ");
    scanf("%d",&n);

    int *a = malloc(n * sizeof(int));
    int *b = malloc(n * sizeof(int));

    long long dot = 0;
    
    for (i = 0; i < n; i++) {
        a[i] = i % 100;
        b[i] = (n - i) % 100;
    }

    double start = omp_get_wtime();

    #pragma omp parallel for reduction(+:dot)
    for (i = 0; i < n; i++) {
        dot += a[i] * b[i];
    }

    double end = omp_get_wtime();
    printf("Minimum Scalar Product: %lld\n", dot);
    printf("Time Taken: %f seconds\n", end - start);

    free(a);
    free(b);
    
    return 0;
}
#include <stdio.h>
#include <omp.h>

int main() {
    int num_threads;

    printf("Enter the number of threads: ");
    scanf("%d", &num_threads);

    printf("\nSequential printing:\n");
    for (int i = 0; i < num_threads; i++) {
        printf("Hello, World - Iteration %d\n", i);
    }

    omp_set_num_threads(num_threads);

    printf("\nParallel printing:\n");
    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        printf("Hello, World from thread %d\n", thread_id);
    }
    return 0;
}
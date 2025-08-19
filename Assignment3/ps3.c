#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() 
{
    int i;
    float scalar = 5.0;
    int n;

    printf("Enter Size: ");
    scanf("%d",&n);
    
    float vector[n];

    for (i = 0; i < n; i++) 
        vector[i] = i * 1.0;
    double start = omp_get_wtime();


    #pragma omp parallel
    {
        #pragma omp for schedule(static, 10) nowait
        for (i = 0; i < n; i++) 
        {
            vector[i] += scalar;
        }
    }
    double end = omp_get_wtime();
    printf("STATIC Schedule with chunk 10 and nowait completed.\n");
    printf("Time taken: %f seconds\n", end - start);
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void vector_scalar_add(int n, float scalar, int chunk, int schedule_type, int use_nowait) {
    int i;
    float *vector = (float *) malloc(n * sizeof(float));

    for (i = 0; i < n; i++)
        vector[i] = i * 1.0;

    double start = omp_get_wtime();

    #pragma omp parallel
    {
        if (schedule_type == 0) {
            if (use_nowait) {
                #pragma omp for schedule(static, chunk) nowait
                for (i = 0; i < n; i++)
                    vector[i] += scalar;
            } else {
                #pragma omp for schedule(static, chunk)
                for (i = 0; i < n; i++)
                    vector[i] += scalar;
            }
        } else {
            if (use_nowait) {
                #pragma omp for schedule(dynamic, chunk) nowait
                for (i = 0; i < n; i++)
                    vector[i] += scalar;
            } else {
                #pragma omp for schedule(dynamic, chunk)
                for (i = 0; i < n; i++)
                    vector[i] += scalar;
            }
        }
    }

    double end = omp_get_wtime();
    double time_taken = end - start;

    if (schedule_type == 0)
        printf("STATIC, chunk=%d, nowait=%d, Time=%f\n", chunk, use_nowait, time_taken);
    else
        printf("DYNAMIC, chunk=%d, nowait=%d, Time=%f\n", chunk, use_nowait, time_taken);

    free(vector);
}

int main() 
{
    int n = 200; 
    float scalar = 5.0;
    int chunk_sizes[] = {1, 5, 10, 20, 50, 100};
    int num_chunks = sizeof(chunk_sizes) / sizeof(chunk_sizes[0]);


    for (int c = 0; c < num_chunks; c++) {
        int chunk = chunk_sizes[c];

        // STATIC without nowait
        vector_scalar_add(n, scalar, chunk, 0, 0);

        // STATIC with nowait
        vector_scalar_add(n, scalar, chunk, 0, 1);

        // DYNAMIC without nowait
        vector_scalar_add(n, scalar, chunk, 1, 0);

        // DYNAMIC with nowait
        vector_scalar_add(n, scalar, chunk, 1, 1);
    }

    return 0;
}

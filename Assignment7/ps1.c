#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) 
{
    int rank, size;
    int m, n; 
    double *A = NULL; 
    double *x = NULL; 
    double *y = NULL; 
    double *local_A = NULL;
    double *local_y = NULL; 
    int rows_per_proc;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) 
    {
        printf("Enter matrix dimensions m x n (m rows, n cols): ");
        fflush(stdout);
        scanf("%d %d", &m, &n);

        A = (double*)malloc(m * n * sizeof(double));
        x = (double*)malloc(n * sizeof(double));
        y = (double*)malloc(m * sizeof(double));

        printf("Initializing matrix and vector...\n");
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                A[i*n + j] = rand() % 10; 
            }
        }
        for (int j = 0; j < n; j++) {
            x[j] = rand() % 10;
        }
    }

    MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    rows_per_proc = m / size;
    int remainder = m % size;
    int local_rows = (rank < remainder) ? rows_per_proc + 1 : rows_per_proc;

    local_A = (double*)malloc(local_rows * n * sizeof(double));
    local_y = (double*)malloc(local_rows * sizeof(double));
    if (rank != 0) x = (double*)malloc(n * sizeof(double));

    MPI_Bcast(x, n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    int *sendcounts = NULL, *displs = NULL;
    if (rank == 0) {
        sendcounts = (int*)malloc(size * sizeof(int));
        displs = (int*)malloc(size * sizeof(int));
        int offset = 0;
        for (int i = 0; i < size; i++) {
            int rows = (i < remainder) ? rows_per_proc + 1 : rows_per_proc;
            sendcounts[i] = rows * n;
            displs[i] = offset;
            offset += rows * n;
        }
    }

    MPI_Scatterv(A, sendcounts, displs, MPI_DOUBLE,
                 local_A, local_rows*n, MPI_DOUBLE,
                 0, MPI_COMM_WORLD);

    for (int i = 0; i < local_rows; i++) {
        local_y[i] = 0.0;
        for (int j = 0; j < n; j++) {
            local_y[i] += local_A[i*n + j] * x[j];
        }
    }

    if (rank == 0) {
        int *recvcounts = sendcounts;
        int *rdispls = displs;
        MPI_Gatherv(local_y, local_rows, MPI_DOUBLE,
                    y, recvcounts, rdispls, MPI_DOUBLE,
                    0, MPI_COMM_WORLD);

        printf("\nResult vector y = A * x:\n");
        for (int i = 0; i < m; i++) {
            printf("%lf ", y[i]);
        }
        printf("\n");

        free(A); free(x); free(y);
        free(sendcounts); free(displs);
    } else {
        MPI_Gatherv(local_y, local_rows, MPI_DOUBLE,
                    NULL, NULL, NULL, MPI_DOUBLE,
                    0, MPI_COMM_WORLD);
        free(x);
    }

    free(local_A);
    free(local_y);

    MPI_Finalize();
    return 0;
}

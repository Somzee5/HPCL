#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int n = 10; 
    int A[10] = {7,9,6,-1,55}; 
    int partial_sum = 0;
    int total_sum = 0;

    int half = n / 2;

    if(rank == 0) {
        for(int i = 0; i < half; i++)
            partial_sum += A[i];

        printf("Process 0 partial sum: %d\n", partial_sum);
        MPI_Send(&partial_sum, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    } 
    else if(rank == 1) {

        for(int i = half; i < n; i++)
            partial_sum += A[i];
        printf("Process 1 partial sum: %d\n", partial_sum);

        int sum_from_p0;
        MPI_Recv(&sum_from_p0, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        total_sum = partial_sum + sum_from_p0;
        printf("Total sum of array elements: %d\n", total_sum);
    }

    MPI_Finalize();
    return 0;
}

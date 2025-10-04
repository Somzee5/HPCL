#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    printf("Process %d out of %d in MPI_COMM_WORLD\n", world_rank, world_size);
    fflush(stdout);  
    
    MPI_Barrier(MPI_COMM_WORLD);  

    MPI_Comm new_comm;
    MPI_Comm_split(MPI_COMM_WORLD, 0, world_rank, &new_comm);

    int new_rank, new_size;
    MPI_Comm_rank(new_comm, &new_rank);
    MPI_Comm_size(new_comm, &new_size);

    printf("Process %d in new communicator group of size %d\n", new_rank, new_size);
    fflush(stdout);  

    MPI_Finalize();
    return 0;
}
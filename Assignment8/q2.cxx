#include <mpi.h>
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int N = 10000000;
    vector<double> A, B;

    if (rank == 0) {
        A.resize(N);
        B.resize(N);
        srand(42);
        for (int i = 0; i < N; i++) {
            A[i] = static_cast<double>(rand()) / RAND_MAX;
            B[i] = static_cast<double>(rand()) / RAND_MAX;
        }
    }

    int local_N = N / size;
    vector<double> local_A(local_N), local_B(local_N);

    MPI_Scatter(A.data(), local_N, MPI_DOUBLE,
                local_A.data(), local_N, MPI_DOUBLE,
                0, MPI_COMM_WORLD);
    MPI_Scatter(B.data(), local_N, MPI_DOUBLE,
                local_B.data(), local_N, MPI_DOUBLE,
                0, MPI_COMM_WORLD);

    double local_dot = 0.0;

    double start = MPI_Wtime();
    for (int i = 0; i < local_N; i++)
        local_dot += local_A[i] * local_B[i];
    double end = MPI_Wtime();

    double global_dot = 0.0;
    MPI_Reduce(&local_dot, &global_dot, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        cout << fixed << setprecision(6);
        cout << "Processes: " << size << endl;
        cout << "Dot Product = " << global_dot << endl;
        cout << "Time = " << end - start << " seconds" << endl;
    }

    MPI_Finalize();
    return 0;
}

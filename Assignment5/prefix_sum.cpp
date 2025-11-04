#include <iostream>
#include <omp.h>
using namespace std;

int main() {
    int N = 16;
    int arr[N], prefix[N];

    // Initializing array
    for(int i = 0; i < N; i++)
        arr[i] = 1;

    int num_threads;
    double start = omp_get_wtime();

    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        int nt = omp_get_num_threads();

        // Let each thread remember total threads
        #pragma omp single
        num_threads = nt;

        int chunk = N / nt;
        int start_i = id * chunk;
        int end_i = (id == nt - 1) ? N : start_i + chunk;

        // 1. Local Prefix Sum
        prefix[start_i] = arr[start_i];
        for(int i = start_i + 1; i < end_i; i++)
            prefix[i] = prefix[i-1] + arr[i];
    }

    // 2. Store last element of each chunk to form partial sums
    int part[num_threads];
    for(int i = 0; i < num_threads; i++) {
        int chunk = N / num_threads;
        int idx = (i+1)*chunk - 1;
        if(i == num_threads - 1) idx = N-1;
        part[i] = prefix[idx];
    }

    // 3. Prefix sum on partial array (serial - size is small)
    for(int i = 1; i < num_threads; i++)
        part[i] += part[i-1];

    // 4. Add back to each chunk (except first thread)
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        if(id == 0) return;  // first chunk already done

        int chunk = N / num_threads;
        int start_i = id * chunk;
        int end_i = (id == num_threads - 1) ? N : start_i + chunk;

        int add_val = part[id-1];

        for(int i = start_i; i < end_i; i++)
            prefix[i] += add_val;
    }

    double end = omp_get_wtime();

    // Print Result
    cout << "Prefix Sum: ";
    for(int i = 0; i < N; i++)
        cout << prefix[i] << " ";
    cout << "\nTime: " << (end - start) << " seconds\n";

    return 0;
}

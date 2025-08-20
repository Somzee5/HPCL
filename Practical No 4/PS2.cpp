#include <iostream>
#include <omp.h>
using namespace std;

long long find(int n) {
    if (n < 2) return n;
    long long x, y;
    #pragma omp task shared(x)
    x = find(n - 1);
    #pragma omp task shared(y)
    y = find(n - 2);
    #pragma omp taskwait
    return x + y;
}

int main() {
    int n = 30;
    double start = omp_get_wtime();
    long long result;

    #pragma omp parallel
    {
        #pragma omp single
        {
            result = find(n);
        }
    }

    double end = omp_get_wtime();
    cout << "Fibonacci(" << n << ") = " << result << endl;
    cout << "Time taken: " << (end - start) << " sec\n";
    return 0;
}

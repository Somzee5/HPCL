#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

long long fib(int n) {
    if (n <= 1) return n;

    long long x, y;

    // create parallel tasks only for bigger n to reduce overhead
    #pragma omp task shared(x) if(n > 20)
    x = fib(n - 1);

    #pragma omp task shared(y) if(n > 20)
    y = fib(n - 2);

    #pragma omp taskwait
    return x + y;
}

int main() {
    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    double start = omp_get_wtime();

    long long result;
    #pragma omp parallel
    {
        #pragma omp single
        {
            cout << "Computing Fibonacci using " 
                 << omp_get_num_threads() << " threads\n";
            result = fib(n);
        }
    }

    double end = omp_get_wtime();

    cout << "Fibonacci(" << n << ") = " << result << "\n";
    cout << "Time taken: " << (end - start) << " sec\n";
    return 0;
}



/*
v[0] = 0;
    v[1] = 1;

    double start = omp_get_wtime();

    #pragma omp parallel
    {
        #pragma omp single
        {
            cout << "Computing series using " << omp_get_num_threads() << " threads\n";
        }

        #pragma omp for schedule(static) nowait
        for (int i = 2; i <= n; i++) {
            #pragma omp critical
            {
                v[i] = v[i - 1] + v[i - 2];
            }
        }
    }

    double end = omp_get_wtime();

    cout << "series: ";
    for (int i = 0; i <= n; i++)
        cout << v[i] << " ";
    cout << "\nTime taken: " << (end - start) << " sec\n";
*/
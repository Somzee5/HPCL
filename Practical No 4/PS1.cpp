#include <iostream>
#include <vector>
#include <omp.h>
using namespace std;

int main() {
    int n ;
    cin>>n;
    vector<long long> v(n + 1, 0);

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

    return 0;
}

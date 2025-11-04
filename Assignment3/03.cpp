#include <omp.h>
#include<bits/stdc++.h>

using namespace std;

int main()
{
    int n;
    cin >> n;
    vector<int> a(n);
    int scalar = 8;

    omp_set_num_threads(5);
    double st = omp_get_wtime();
    #pragma omp parallel for schedule(dynamic, 10)
    for(int i=0;i<n;i++)
    {
        a[i] += scalar;
    }

    double ed = omp_get_wtime();
    cout << ed - st;
}
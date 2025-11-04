#include <omp.h>
#include<bits/stdc++.h>

using namespace std;

int main()
{
    int n;
    cin >> n;


    vector<vector<int>> a(n, vector<int>(n, 1));
    vector<vector<int>> b(n, vector<int>(n, 1));
    vector<vector<int>> c(n, vector<int>(n, 1));

    double st = omp_get_wtime();


    omp_set_num_threads(15);
    #pragma omp parallel for collapse(2)
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            c[i][j] = a[i][j] + b[i][j];
        }
    }

    double ed = omp_get_wtime();

    cout << ed - st;
}
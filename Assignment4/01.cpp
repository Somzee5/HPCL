#include<bits/stdc++.h>
#include<omp.h>

using namespace std;

int fibo(int n)
{
    if(n<=1)
        return n;

    int a, b;
    
    #pragma omp task shared(a)
    a = fibo(n - 1);

    #pragma omp task shared(b)
    b = fibo(n - 2);

    #pragma omp taskwait

    return a + b;
}


int main()
{
    double st = omp_get_wtime();

    cout << fibo(10) << endl;

    double ed = omp_get_wtime();

    cout << ed - st << endl;
}
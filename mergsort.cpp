#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

void mergeArr(int arr[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    vector<int> L(n1), R(n2);
    for(int i = 0; i < n1; i++) L[i] = arr[l + i];
    for(int i = 0; i < n2; i++) R[i] = arr[m + 1 + i];

    int i = 0, j = 0, k = l;
    while(i < n1 && j < n2)
        arr[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];

    while(i < n1) arr[k++] = L[i++];
    while(j < n2) arr[k++] = R[j++];
}

void mergeSort(int arr[], int l, int r) {
    if(l >= r) return;

    int m = (l + r) / 2;

    #pragma omp task shared(arr)
    mergeSort(arr, l, m);

    #pragma omp task shared(arr)
    mergeSort(arr, m+1, r);

    #pragma omp taskwait
    mergeArr(arr, l, m, r);
}

int main() {
    int n = 16;
    int arr[n];

    for(int i = 0; i < n; i++) arr[i] = rand() % 100;

    cout << "Before Sort: ";
    for(int x : arr) cout << x << " ";
    cout << endl;

    double st = omp_get_wtime();
    #pragma omp parallel
    {
        #pragma omp single
        mergeSort(arr, 0, n-1);
    }
    double en = omp_get_wtime();

    cout << "After Merge Sort: ";
    for(int x : arr) cout << x << " ";
    cout << "\nTime: " << en - st << " sec\n";
}

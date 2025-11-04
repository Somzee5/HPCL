#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

int partitionArr(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for(int j = low; j < high; j++) {
        if(arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i+1], arr[high]);
    return i+1;
}

void quickSort(int arr[], int low, int high) {
    if(low < high) {
        int pi = partitionArr(arr, low, high);

        #pragma omp task shared(arr)
        quickSort(arr, low, pi - 1);

        #pragma omp task shared(arr)
        quickSort(arr, pi + 1, high);

        #pragma omp taskwait
    }
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
        quickSort(arr, 0, n-1);
    }
    double en = omp_get_wtime();

    cout << "After Quick Sort: ";
    for(int x : arr) cout << x << " ";
    cout << "\nTime: " << en - st << " sec\n";
}

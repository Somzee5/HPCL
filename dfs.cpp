#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

void dfsUtil(int u, vector<vector<int>> &adj, vector<int> &visited) {
    visited[u] = 1;
    cout << u << " ";

    #pragma omp parallel for
    for(int i = 0; i < adj[u].size(); i++) {
        int v = adj[u][i];
        if(!visited[v]) {
            #pragma omp task shared(adj, visited)
            dfsUtil(v, adj, visited);
        }
    }

    #pragma omp taskwait
}

void dfs_parallel(vector<vector<int>> &adj, int start) {
    int n = adj.size();
    vector<int> visited(n, 0);

    cout << "DFS Order: ";

    #pragma omp parallel
    {
        #pragma omp single
        dfsUtil(start, adj, visited);
    }

    cout << endl;
}

int main() {
    int n = 6; 
    vector<vector<int>> adj(n);

    adj[0] = {1, 2};
    adj[1] = {0, 3, 4};
    adj[2] = {0, 4};
    adj[3] = {1, 5};
    adj[4] = {1, 2, 5};
    adj[5] = {3, 4};

    double st = omp_get_wtime();
    dfs_parallel(adj, 0);
    double en = omp_get_wtime();

    cout << "Time: " << en - st << " sec\n";
}

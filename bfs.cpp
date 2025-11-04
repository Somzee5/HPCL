#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

void bfs_parallel(vector<vector<int>> &adj, int start) {
    int n = adj.size();
    vector<int> visited(n, 0);
    queue<int> q;

    visited[start] = 1;
    q.push(start);

    cout << "BFS Order: ";

    while(!q.empty()) {
        int u = q.front();
        q.pop();

        cout << u << " ";

        // Process neighbors in parallel
        #pragma omp parallel for
        for(int i = 0; i < adj[u].size(); i++) {
            int v = adj[u][i];
            if(!visited[v]) {
                // Critical section to prevent data race
                #pragma omp critical
                {
                    if(!visited[v]) {
                        visited[v] = 1;
                        q.push(v);
                    }
                }
            }
        }
    }
    cout << endl;
}

int main() {
    int n = 6; // number of nodes
    vector<vector<int>> adj(n);

    adj[0] = {1, 2};
    adj[1] = {0, 3, 4};
    adj[2] = {0, 4};
    adj[3] = {1, 5};
    adj[4] = {1, 2, 5};
    adj[5] = {3, 4};

    double st = omp_get_wtime();
    bfs_parallel(adj, 0);
    double en = omp_get_wtime();

    cout << "Time: " << en - st << " sec\n";
}

#pragma once
#include <iostream>
#include <list>
#include <iomanip>
const double INF = 1000000;
using namespace std;
inline double drand(double d, double u) { return d + (static_cast<double>(rand()) / RAND_MAX) * (u - d); }
template <class T1, class T2> class Graph {
    T1 V;
    list<pair<T1, T2> >* adjacencyList;
public:
    // A constructor that builds a graph according to the given parameters of the number of
    // vertices, density and range
    Graph(T1 V, T2 density, T2 dnLim, T2 upLim) : V(V), adjacencyList(new list<pair<T1, T2> >[V]) {
        for (T1 i = 0; i < V; i++)
            for (T1 e = 0; e < V * density; e++)
                addEdge(i, rand() % V, drand(dnLim, upLim));
    }
    ~Graph() { delete[] adjacencyList; }

    // Add an edge in an undirected graph
    void addEdge(T1 u, T1 v, T2 weight) {
        adjacencyList[u].push_back(make_pair(v, weight));
        adjacencyList[v].push_back(make_pair(u, weight));
    }

    // Print the adjacency list representation of graph
    void printAdjacencyList() {
        for (T1 v = 0; v < V; v++) {
            cout << "Vertex " << v << ':';
            for (const pair<T1, T2>& p : adjacencyList[v])
                cout << " -> " << setw(2) << p.first << ":w=" << setprecision(3) << p.second;
            cout << endl;
        }
    }

    // Minimum distance among unvisited vertices
    T1 minDistance(T2* dist, bool* visited) {
        T2 min = INF;
        T1 index = 0;
        for (T1 i = 0; i < V; i++)
            if (dist[i] < min && !visited[i]) { min = dist[i]; index = i; }
        return index;
    }

    // Typical representation of Dijkstra's algorithm
    T2** dijkstra(T1 source) {
        T2* dist = new T2[V], * prev = new T2[V];
        bool* visited = new bool[V];
        for (T1 i = 0; i < V; i++) {
            dist[i] = INF;
            prev[i] = -1;
            visited[i] = false;
        }
        dist[source] = 0;
        prev[source] = source;
        for (T1 i = 0; i < V - 1; i++) {
            T1 u = minDistance(dist, visited);
            visited[u] = true;
            for (list<pair<T1, T2> > ::iterator it = adjacencyList[u].begin(); it != adjacencyList[u].end(); it++)
                if (!visited[it->first] && dist[u] != INF && dist[it->first] > dist[u] + it->second) {
                    dist[it->first] = dist[u] + it->second;
                    prev[it->first] = u;
                }
        }
        return new T2 * [2]{ dist, prev };
    }
};

// The function of calculating the arithmetic mean of only positive numbers not equal
// to infinity, taking into account their number in the array
template <class T1, class T2> T2 calcAveragePositiveDistance(T2** arr, T1 size) {
    if (size == 0) return T2(0);
    T2 positiveSum = 0;
    T1 positiveCount = size;
    for (T1 i = 0; i < size; i++)
        if (arr[1][i] >= 0 && arr[0][i] < INF) positiveSum += arr[0][i];
        else positiveCount--;
    return positiveSum / positiveCount;
}
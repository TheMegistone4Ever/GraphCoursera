#pragma once
#include <iostream>
#include <list>
#include <iomanip>
#include <cctype>
const double INF = 1000000;
using namespace std;
inline double drand(double d, double u) { return d + (static_cast<double>(rand()) / RAND_MAX) * (u - d); }

struct DisjointSets {
    int n, * parent;
    DisjointSets(int n) {
        this->n = n;
        parent = new int[n + 1];
        for (int i = 0; i <= n; i++) parent[i] = i;
    }

    // Find and return the parent of a x node
    inline int find(int x) { return parent[x] != x ? find(parent[x]) : x; }

    // Union two elements
    inline void merge(int x, int y) { parent[find(y)] = find(x); }
};

template <class T1, class T2> class Graph {
    T1 V;
    list<pair<T1, T2> >* adjacencyList;
    list<pair<T2, pair<T1, T1> > > edges;
public:
    // A constructor that builds a graph according to the given parameters of the number of
    // vertices, density and range
    Graph(T1 V = 0, T2 density = 0, T2 dnLim = 0, T2 upLim = 0) : V(V), adjacencyList(new list<pair<T1, T2> >[V]) {
        if (dnLim > upLim) swap(dnLim, upLim);
        for (T1 i = 0; i < V; i++)
            for (T1 e = 0; e < V * min(T2(1), max(T2(0), density)); e++)
                addEdge(i, T1(rand() % V), drand(dnLim, upLim));
    }
    ~Graph() { delete[] adjacencyList; }

    // Add an edge in an undirected graph, return true if edge was added succesfully
    bool addEdge(T1 u, T1 v, T2 weight) {
        if (u == v) return false; // No loops in this graph

        // No dublicates in this graph
        for (list<pair<T1, T2> >::iterator it = adjacencyList[u].begin(); it != adjacencyList[u].end(); it++)
            if (it->first == v) return false;

        adjacencyList[u].push_back(make_pair(v, weight));
        adjacencyList[v].push_back(make_pair(u, weight));
        edges.push_back(make_pair(weight, make_pair(u, v)));
        return true;
    }

    // Print the adjacency list representation of graph
    void printAdjacencyList() {
        T1 w = static_cast<T1>(log10(V)) + 1;
        for (T1 v = 0; v < V; v++) {
            cout << "Vertex " << setw(w) << v << ':';
            for (const pair<T1, T2>& p : adjacencyList[v])
                cout << " -> " << setw(w) << p.first << ":w=" << fixed << setprecision(3) << p.second;
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

    // Typical representation of Dijkstra's algorithm (breadth)
    T2** dijkstra(T1 source) {
        source %= V;
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
            for (list<pair<T1, T2> >::iterator it = adjacencyList[u].begin(); it != adjacencyList[u].end(); it++)
                if (!visited[it->first] && dist[u] != INF && dist[it->first] > dist[u] + it->second) {
                    dist[it->first] = dist[u] + it->second;
                    prev[it->first] = u;
                }
        }
        return new T2 * [2]{ dist, prev };
    }

    // Checks if a graph is fully connected
    bool isFullyConnected() {
        T2** arr = dijkstra(0);
        for (T1 i = 0; i < V; i++) if (arr[1][i] < 0) return false;
        return true;
    }

    // Function to build minimum spanning forest (Kruskal's)
    Graph kruskalSTP(T2& weightSTP) {
        Graph stp(V);
        DisjointSets ds(V);
        edges.sort(); // Iterate through all sorted edges
        for (list<pair<T2, pair<T1, T1> > >::iterator it = edges.begin(); it != edges.end(); it++) {
            cout << "S EDGES: " << it->first << "/" << it->second.first << "/" << it->second.second << endl;
        }
        for (list<pair<T2, pair<T1, T1> > >::iterator it = edges.begin(); it != edges.end(); it++) {
            int u = it->second.first, v = it->second.second, set_u = ds.find(u), set_v = ds.find(v);

            // Check if the selected edge is creating a cycle or not
            if (set_u != set_v) {
                // Current edge will be in the MST
                cout << u << " -MST- " << v << endl;
                stp.addEdge(u, v, it->first);
                weightSTP += it->first;
                ds.merge(set_u, set_v);
            }
        
        }
        return stp;
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
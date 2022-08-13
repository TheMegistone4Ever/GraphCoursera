#pragma once
#include <iostream>
#include <iomanip>
#include <vector>
#include <set>
#include <algorithm>
const double INF = 1000000;
const int COLORS = 3;
using namespace std;
inline double drand(double d, double u) { return d + (static_cast<double>(rand()) / RAND_MAX) * (u - d); }
enum color {RED, GREEN, BLUE};

template <class T1, class T2> class Graph {
    struct DisjointSets {
        T1 size, * parent;
        DisjointSets(T1 size) {
            this->size = size;
            parent = new T1[size + 1];
            for (T1 i = 0; i <= size; i++) parent[i] = i;
        }

        // Find and return the parent of a x node
        inline T1 find(T1 x) { return parent[x] != x ? find(parent[x]) : x; }

        // Union two elements
        inline void merge(T1 x, T1 y) { parent[find(y)] = find(x); }
    };

    struct edge {
        T2 weight;
        T1 u, v;
        color c;
        edge(T2 weight, T1 u, T1 v, color c) : weight(weight), u(u), v(v), c(c) {}
        bool operator<(const edge& a) const { return weight < a.weight; }
    };

    T1 V;
    vector<pair<T1, T2> >* adjacencyList;
    vector<edge> edges, redEdges, greenEdges, blueEdges; // (weight, u, v, color)

public:
    // A constructor that builds a graph according to the given parameters of the number of
    // vertices, density and range
    Graph(T1 V = 0, T2 density = 0, T2 dnLim = 0, T2 upLim = 0) : V(V), adjacencyList(new vector<pair<T1, T2> >[V]) {
        if (dnLim > upLim) swap(dnLim, upLim);
        for (T1 i = 0; i < V; i++)
            for (T1 e = 0; e < V * min(T2(1), max(T2(0), density)); e++)
                addEdge({ drand(dnLim, upLim), i, T1(rand() % V), static_cast<color>(rand() % COLORS) });
    }
    ~Graph() { /*Here the std::list destructor is called for everything automatically*/ }

    // Add an edge in an undirected graph, return true if edge was added succesfully
    bool addEdge(edge e) {
        if (e.u == e.v) return false; // No loops in this graph

        // No dublicates in this graph
        for (typename vector<pair<T1, T2> >::iterator it = adjacencyList[e.u].begin(); it != adjacencyList[e.u].end(); it++)
            if (it->first == e.v) return false;

        adjacencyList[e.u].push_back(make_pair(e.v, e.weight));
        adjacencyList[e.v].push_back(make_pair(e.u, e.weight));
        edges.push_back(e);

        switch (e.c) {
        case RED:
            redEdges.push_back(e);
            break;
        case GREEN:
            greenEdges.push_back(e);
            break;
        case BLUE:
            blueEdges.push_back(e);
            break;
        default:
            cout << "ERROR COLOR: " << e.c << endl;
        }
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

    // Typical representation of Dijkstra's algorithm (breadth search)
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
            for (typename vector<pair<T1, T2> >::iterator it = adjacencyList[u].begin(); it != adjacencyList[u].end(); it++)
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
    Graph kruskalSTP(T2& weightSTP, bool red = true, bool green = true, bool blue = true) {

        set<edge> s;
        if (red) s.insert(redEdges.begin(), redEdges.end());
        if (green) s.insert(greenEdges.begin(), greenEdges.end());
        if (blue) s.insert(blueEdges.begin(), blueEdges.end());

        // if the graph is not fully connected, then it cannot have a minimum spanning tree
        if (!isFullyConnected()) return Graph();
        Graph stp(V);
        DisjointSets ds(V);

        for (typename set<edge>::iterator it = s.begin(); it != s.end(); it++) {
            T1 parentU = ds.find(it->u), parentV = ds.find(it->v);
            // Check if the selected edge is creating a cycle or not and, if not, add to MST
            if (parentU != parentV) {
                stp.addEdge({ it->weight, it->u, it->v, it->c });
                weightSTP += it->weight;
                ds.merge(parentU, parentV);
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
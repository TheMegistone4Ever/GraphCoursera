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

template <class T1, class T2> class Graph {
    enum color { RED, GREEN, BLUE };
    
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
    vector<edge>* adjacencyList, edges, redEdges, greenEdges, blueEdges; // (weight, u, v, color)

public:
    vector<edge>* getAdjacencyList() { return adjacencyList; }

    // A constructor that builds a graph according to the given parameters of the number of
    // vertices, density and range
    Graph(T1 V = 0, T2 density = 0, T2 dnLim = 0, T2 upLim = 0) : V(V), adjacencyList(new vector<edge>[V]) {
        if (dnLim > upLim) swap(dnLim, upLim);
        for (T1 i = 0; i < V; i++)
            for (T1 e = 0; e < V * min(static_cast<T2>(1), max(static_cast<T2>(0), density)); e++)
                addEdge({ drand(dnLim, upLim), i, static_cast<T1>(rand() % V), static_cast<color>(rand() % COLORS) });
    }
    ~Graph() {
        // Here the std::vector destructor is called for everything automatically
    }

    // Add an edge in an undirected graph, return true if edge was added succesfully
    bool addEdge(edge e) {
        if (e.u == e.v) return false; // No loops in this graph

        // No dublicates in this graph
        for (typename vector<edge>::iterator it = adjacencyList[e.u].begin(); it != adjacencyList[e.u].end(); it++)
            if (it->u == e.v) return false;

        adjacencyList[e.u].push_back(e);
        adjacencyList[e.v].push_back(e);
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
            for (const edge& p : adjacencyList[v])
                cout << " -> " << setw(w) << p.u << ":w=" << fixed << setprecision(3) << p.weight << "c=" << p.c;
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

    vector<edge>* createAdjacencyListFromSet(set<edge> s) {
        vector<edge>* resAL = new vector<edge>[V];
        for (typename set<edge>::iterator it = s.begin(); it != s.end(); it++) {
            edge e = { it->weight, it->u, it->v, it->c };
            resAL[it->u].push_back(e);
            resAL[it->v].push_back(e);
        }
        return resAL;
    }

    // Typical representation of Dijkstra's algorithm (breadth search)
    pair<T2*, T1*> dijkstra(T1 source, vector<edge>* al) {
        T2* dist = new T2[V];
        T1* prev = new T1[V];
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
            for (typename vector<edge>::iterator it = al[u].begin(); it != al[u].end(); it++)
                if (!visited[it->u] && dist[u] != INF && dist[it->u] > dist[u] + it->weight) {
                    dist[it->u] = dist[u] + it->weight;
                    prev[it->u] = u;
                }
        }
        return make_pair(dist, prev);
    }

    // Checks if a graph is fully connected
    bool isFullyConnected(vector<edge>* al) {
        pair<T2*, T1*> p = dijkstra(0, al);
        for (T1 i = 0; i < V; i++) if (p.second[i] < 0) return false;
        return true;
    }

    // Function to build minimum spanning forest (Kruskal's)
    Graph kruskalMST(T2& weightMST, bool red = true, bool green = true, bool blue = true) {
        set<edge> s;
        if (red && green && blue) s.insert(edges.begin(), edges.end());
        else {
            if (red) s.insert(redEdges.begin(), redEdges.end());
            if (green) s.insert(greenEdges.begin(), greenEdges.end());
            if (blue) s.insert(blueEdges.begin(), blueEdges.end());
        }

        // If the graph is not fully connected, then it cannot have a minimum spanning tree
        if (red && green && blue){
            if (!isFullyConnected(adjacencyList)) return Graph();
        } else if (!isFullyConnected(createAdjacencyListFromSet(s))) return Graph();
        Graph stp(V);
        DisjointSets ds(V);

        for (typename set<edge>::iterator it = s.begin(); it != s.end(); it++) {
            T1 parentU = ds.find(it->u), parentV = ds.find(it->v);
            // Check if the selected edge is creating a cycle or not and, if not, add to MST
            if (parentU != parentV) {
                stp.addEdge({ it->weight, it->u, it->v, it->c });
                weightMST += it->weight;
                ds.merge(parentU, parentV);
            }
        }
        return stp;
    }
};

// The function of calculating the arithmetic mean of only positive numbers not equal
// to infinity, taking into account their number in the array
template <class T1, class T2> T2 calcAveragePositiveDistance(pair<T2*, T1*> p, T1 size) {
    if (size == 0) return static_cast<T2>(0);
    T2 positiveSum = 0;
    T1 positiveCount = size;
    for (T1 i = 0; i < size; i++)
        if (p.second[i] >= 0 && p.first[i] < INF) positiveSum += p.first[i];
        else positiveCount--;
    return positiveSum / positiveCount;
}
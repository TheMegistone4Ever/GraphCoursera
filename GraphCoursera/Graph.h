#pragma once
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <fstream>
const double INF = 99999;
const int COLORS = 3;
using namespace std;
inline double drand(double d, double u) { return d + (static_cast<double>(rand()) / RAND_MAX) * (u - d); }
enum color { RED, GREEN, BLUE };

template <class T1, class T2> class Graph {
    struct DisjointSets {
        T1 size, * parent;
        DisjointSets(T1 size) {
            this->size = size;
            parent = new T1[size + 1];
            for (T1 i = 0; i <= size; i++) parent[i] = i;
        }
        ~DisjointSets() { delete[] parent; }

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
    Graph(const char* filename) {
        ifstream file(filename);
        if (file) {
            istream_iterator<int> start(file), end;
            vector<int> data(start, end);
            V = *data.begin();
            adjacencyList = new vector<edge>[V];
            for (auto it = next(data.begin()); it != data.end(); it = next(it)) {
                T1 u = *it;
                T1 v = *(it = next(it));
                T2 w = *(it = next(it));
                addEdge({ w, u, v, BLUE });
            }
            cout << "File was readed succesfully from file: " << filename << endl;
            file.close();
        } else cerr << "Error: file could not be opened!" << endl;
    }
    ~Graph() {
        // Here the std::vector destructor is called for everything automatically
    }

    // Add an edge in an undirected graph, return true if edge was added succesfully
    bool addEdge(edge e) {
        if (e.u == e.v) return false; // No loops in this graph

        // No dublicates in this graph
        for (auto it = adjacencyList[e.u].begin(); it != adjacencyList[e.u].end(); it++)
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
            cerr << "ERROR COLOR: " << e.c << endl;
        }
        return true;
    }

    // Print the adjacency list representation of graph
    void printAdjacencyList() {
        T1 w = static_cast<T1>(log10(V)) + 1;
        for (T1 v = 0; v < V; v++) {
            cout << "Vertex " << setw(w) << v << ':';
            for (const edge& e : adjacencyList[v])
                cout << " -> " << setw(w) << e.u << ":w=" << fixed << setprecision(3) << e.weight << "c=" << e.c;
            cout << endl;
        }
    }

    // Creating adjacency list from set
    vector<edge>* createAdjacencyListFromVector(vector<edge> s) {
        vector<edge>* resAL = new vector<edge>[V];
        for (auto it = s.begin(); it != s.end(); it++) {
            edge e = { it->weight, it->u, it->v, it->c };
            resAL[it->u].push_back(e);
            resAL[it->v].push_back(e);
        }
        return resAL;
    }

    // Minimum distance among unvisited vertices
    T1 minDistance(T2* dist, bool* visited) {
        T2 min = INF;
        T1 index = 0;
        for (T1 i = 0; i < V; i++)
            if (!visited[i] && dist[i] < min) min = dist[i], index = i;
        return index;
    }

    // Typical representation of Dijkstra's algorithm (breadth search)
    pair<T2*, T1*> dijkstra(T1 source, vector<edge>* al = nullptr) {
        if (al == nullptr) al = adjacencyList;
        T2* dist = new T2[V];
        T1* prev = new T1[V];
        bool* visited = new bool[V];
        for (T1 i = 0; i < V; i++) dist[i] = INF, prev[i] = -1, visited[i] = false;
        dist[source] = 0, prev[source] = source;
        for (T1 i = 0; i < V - 1; i++) {
            T1 nearest = minDistance(dist, visited);
            visited[nearest] = true;
            for (auto adj = al[nearest].begin(); adj != al[nearest].end(); adj++)
                if (!visited[adj->v] && dist[nearest] != INF && dist[nearest] + adj->weight < dist[adj->v]) {
                    dist[adj->v] = dist[nearest] + adj->weight;
                    prev[adj->v] = nearest;
                }
        }
        return make_pair(dist, prev);
    }

    // Checks if a graph is fully connected
    bool isFullyConnected(vector<edge>* al = nullptr) {
        if (al == nullptr) al = adjacencyList;
        pair<T2*, T1*> p = dijkstra(0, al);
        for (T1 i = 0; i < V; i++) if (p.second[i] < 0) return false;
        return true;
    }

    // Function to build minimum spanning forest (Kruskal's)
    Graph kruskalMST(T2& weightMST, bool red = true, bool green = true, bool blue = true) {
        vector<edge> s;
        if (red && green && blue) s.insert(s.end(), edges.begin(), edges.end());
        else {
            if (red) s.insert(s.end(), redEdges.begin(), redEdges.end());
            if (green) s.insert(s.end(), greenEdges.begin(), greenEdges.end());
            if (blue) s.insert(s.end(), blueEdges.begin(), blueEdges.end());
        }
        // If the graph is not fully connected, then it cannot have a minimum spanning tree
        if (red && green && blue) { if (!isFullyConnected(adjacencyList)) return Graph();
        } else if (!isFullyConnected(createAdjacencyListFromVector(s))) return Graph();
        sort(s.begin(), s.end());
        Graph stp(V);
        DisjointSets ds(V);
        for (auto it = s.begin(); it != s.end(); it++) {
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
    if (size == 0) return 0;
    T2 positiveSum = 0;
    T1 positiveCount = size;
    for (T1 i = 0; i < size; i++)
        if (p.second[i] >= 0 && p.first[i] < INF) positiveSum += p.first[i];
        else positiveCount--;
    return positiveSum / positiveCount;
}
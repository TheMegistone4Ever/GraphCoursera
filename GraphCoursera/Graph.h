#pragma once
#include <iostream>
#include <list>
#include <iterator>
#include <iomanip>
const double INF = 1000000;
using namespace std;
inline double drand(double d, double u) { return d + (static_cast<double>(rand()) / RAND_MAX) * (u - d); }
class Graph {
    int V;
    list<pair<int, double> >* adjacencyList;
public:
    Graph(int V, double density, double dnLim, double upLim) : V(V), adjacencyList(new list<pair<int, double> >[V]) {
        for (int i = 0; i < V; i++)
            for (int e = 0; e < V * density; e++)
                addEdge(i, rand() % V, drand(dnLim, upLim));
    }
    ~Graph() { delete[] adjacencyList; }

    // add an edge in an undirected graph
    void addEdge(int u, int v, double weight) {
        adjacencyList[u].push_back(make_pair(v, weight));
        adjacencyList[v].push_back(make_pair(u, weight));
    }

    // print the adjacency list representation of graph
    void printAdjacencyList() {
        for (int v = 0; v < V; v++) {
            cout << "Vertex " << v << ':';
            for (const pair<int, double>& p : adjacencyList[v])
                cout << " -> " << setw(2) << p.first << ":w=" << setprecision(3) << p.second;
            cout << endl;
        }
    }

    int minDistance(double* dist, bool* visited) {
        double min = INF, index = 0;
        for (int i = 0; i < V; i++)
            if (dist[i] < min && !visited[i]) { min = dist[i]; index = i; }
        return index;
    }

    double** dijkstra(int source) {
        double* dist = new double[V], * prev = new double[V];
        bool* visited = new bool[V];
        for (int i = 0; i < V; i++) {
            dist[i] = INF;
            prev[i] = -1;
            visited[i] = false;
        }
        dist[source] = 0;
        prev[source] = source;
        for (int i = 0; i < V - 1; i++) {
            int u = minDistance(dist, visited);
            visited[u] = true;
            for (list<pair<int, double> >::iterator it = adjacencyList[u].begin(); it != adjacencyList[u].end(); it++)
                if (!visited[it->first] && dist[u] != INF && dist[it->first] > dist[u] + it->second) {
                    dist[it->first] = dist[u] + it->second;
                    prev[it->first] = u;
                }
        }
        double** res = new double* [2];
        res[0] = dist;
        res[1] = prev;
        return res;
    }
};
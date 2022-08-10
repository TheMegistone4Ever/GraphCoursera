#pragma once
#include<iostream>
#include <list>
#include <iterator>
using namespace std;

inline double drand(double d, double u) { return d + (static_cast<double>(rand()) / RAND_MAX) * (u - d); }

class Graph {
    int V;
    list<pair<int, int> >* adjacencyList;
public:
    Graph(int V, double density, double dnLim, double upLim) : V(V), adjacencyList(new list<pair<int, int> >[V]) {
        
        // DO V * DENSITY in limits 

    }


    // add an edge in an undirected graph
    void addEdge(int u, int v, double weight) {
        adjacencyList[u].push_back(make_pair(v, weight));
        adjacencyList[v].push_back(make_pair(u, weight));
    }


    // print the adjacency list representation of graph
    void printAdjacencyList() {
        for (int v = 0; v < V; v++) {
            cout << "Vertex " << v << ':';
            for (pair<int, int> p : adjacencyList[v]) cout << " -> " << p.first << ":w=" << p.second;
            cout << endl;
        }
    }


    int* dijkstra(int source) {
        return NULL;
    }
};
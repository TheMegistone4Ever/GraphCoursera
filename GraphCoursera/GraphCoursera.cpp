// Peer-graded Assignment: Homework 2: Implement Dijkstra's Algorithm,
// C++ For C Programmers, Part A, Week 3,
// by Nickita Kyselyov, NTUU KPI nb. Igor Sikorsky, 2nd grade, FICT, IS-13,
// 11.08.2022
#include "Graph.h"
int main(int argc, char const* argv[]) {
    srand(static_cast<unsigned int>(time(nullptr)));
    int V; cout << "Input V: "; cin >> V;
    double density; cout << "Input density: "; cin >> density;
    double dnLim; cout << "Input dnLim: "; cin >> dnLim;
    double upLim; cout << "Input upLim: "; cin >> upLim;
    int source; cout << "Input source vertice: "; cin >> source;
    Graph<int, double> graph(V, density, dnLim, upLim);
    // Set graph for manual processing
    /*graph.addEdge(0, 1, 2);
    graph.addEdge(0, 2, 4);
    graph.addEdge(1, 2, 3);
    graph.addEdge(1, 3, 4);
    graph.addEdge(1, 4, 5);
    graph.addEdge(2, 3, 2);
    graph.addEdge(2, 4, 3);
    graph.addEdge(3, 4, 1);*/
    graph.printAdjacencyList();
    double** dijkstraInfo = graph.dijkstra(source);
    double avgDist = calcAveragePositiveDistance(dijkstraInfo, V);

    // The output of all the necessary information obtained during the operation of Dijkstra's algorithm
    for (int i = 0; i < V; i++)
        cout << source << "\t->\t"
        << i << "\tMin dist:\t" << setprecision(5) << dijkstraInfo[0][i]
        << "\tV Prev:\t" << dijkstraInfo[1][i] << endl;

    cout << "AVERAGE MIN POSITIVE DISTANCE: " << setprecision(5) << avgDist << endl;
    return 0;
}
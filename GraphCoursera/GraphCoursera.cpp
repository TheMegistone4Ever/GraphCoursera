// Graph class from Peer-graded Assignment: Homework 2: Implement Dijkstra's Algorithm,
// Task: 
// C++ For C Programmers, Part A, Week 4,
// by Nickita Kyselyov, NTUU KPI nb. Igor Sikorsky, 2nd grade, FICT, IS-13,
// 11.08.2022
#include "Graph.h"
int main(int argc, char const* argv[]) {
    srand(static_cast<unsigned int>(time(nullptr)));
    int V; cout << "Input V: "; cin >> V; if (V < 0) V = abs(V);
    double density; cout << "Input density (from 0 to 1): "; cin >> density;
    double dnLim; cout << "Input dnLim for random: "; cin >> dnLim;
    double upLim; cout << "Input upLim for random: "; cin >> upLim;
    int source; cout << "Input source vertice (from 0 to " << V - 1 << "): "; cin >> source;
    Graph<int, double> graph(V, density, dnLim, upLim);
    graph.printAdjacencyList();
    //double** dijkstraInfo = graph.dijkstra(source);
    //double avgDist = calcAveragePositiveDistance(dijkstraInfo, V);

    // The output of all the necessary information obtained during the operation of Dijkstra's algorithm
    /*for (int i = 0; i < V; i++)
        cout << source << "\t->\t"
        << i << "\tMin dist:\t" << setprecision(5) << dijkstraInfo[0][i]
        << "\tV Prev:\t" << dijkstraInfo[1][i] << endl;*/

    //cout << "AVERAGE MIN POSITIVE DISTANCE: " << setprecision(5) << avgDist << endl;

    cout << "GRAPH IS FULLY CONNECTED: " << boolalpha << graph.isFullyConnected() << endl;

    double weightSTP = 0;
    Graph<int, double> stp = graph.kruskalSTP(weightSTP, 0, 1, 1);
    stp.printAdjacencyList();
    cout << "WEIGHT OF STP: " << fixed << setprecision(3) << weightSTP << endl;
    cout << "STP GRAPH IS FULLY CONNECTED: " << boolalpha << stp.isFullyConnected() << endl;
    return EXIT_SUCCESS;
}
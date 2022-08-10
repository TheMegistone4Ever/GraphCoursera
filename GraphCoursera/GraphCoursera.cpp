// ,
// by Nickita Kyselyov, NTUU KPI nb. Igor Sikorsky, 1st grade, FICT, IS-13
#include "Graph.h"

int main(int argc, char const* argv[]) {
    srand(time(NULL));
    int V; cout << "input V: "; cin >> V;
    double density; cout << "input density: "; cin >> density;
    double dnLim; cout << "input dnLim: "; cin >> dnLim;
    double upLim; cout << "input upLim: "; cin >> upLim;

    Graph graph(V, density, dnLim, upLim);
    graph.addEdge(0, 1, 2);
    graph.addEdge(0, 2, 4);
    graph.addEdge(1, 2, 3);
    graph.addEdge(1, 3, 4);
    graph.addEdge(1, 4, 5);
    graph.addEdge(2, 3, 2);
    graph.addEdge(2, 4, 3);
    graph.addEdge(3, 4, 1);
    graph.printAdjacencyList();
    graph.dijkstra(0);

    return 0;
}
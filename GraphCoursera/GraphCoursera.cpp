// ,
// by Nickita Kyselyov, NTUU KPI nb. Igor Sikorsky, 1st grade, FICT, IS-13,
// 11.08.2022
#include "Graph.h"
int main(int argc, char const* argv[]) {
    srand(time(NULL));
    int V; cout << "Input V: "; cin >> V;
    double density; cout << "Input density: "; cin >> density;
    double dnLim; cout << "Input dnLim: "; cin >> dnLim;
    double upLim; cout << "Input upLim: "; cin >> upLim;
    int source; cout << "Input source vertice: "; cin >> source;

    Graph graph(V, density, dnLim, upLim);
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
    for (int i = 0; i < V; i++)
        cout << source << "\t->\t"
        << i << "\tMin dist:\t" << setprecision(5) << dijkstraInfo[0][i]
        << "\tV Prev:\t" << dijkstraInfo[1][i] << endl;
    return 0;
}
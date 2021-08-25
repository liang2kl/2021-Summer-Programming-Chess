#ifndef GRAPH_H
#define GRAPH_H

#include <QVector>
#include <QDebug>

class Graph {
private:
    QVector<QVector<int>> adjLists;

public:
    Graph(int v) {
        adjLists = QVector<QVector<int>>(v + 1);
    }
    void addEdge(int from, int to) {
        adjLists[from].push_back(to);
    }

    // dfs
    bool isConnected(int from, int to, QVector<int> visited = {}) const {
        visited.append(from);

        for (auto edgeTo : adjLists[from]) {
            if (visited.contains(edgeTo)) { continue; }
            if (edgeTo == to) { return true; }

            if (isConnected(edgeTo, to, visited)) {
                return true;
            }
        }

        return false;
    }
};

#endif // GRAPH_H

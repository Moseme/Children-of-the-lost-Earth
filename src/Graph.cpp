#include "Graph.h"
#include <iostream>

Graph::Graph(int maxVertices) : capacity(maxVertices), numVertices(0) {
    vertices = new Vertex[capacity];
}

Graph::~Graph() {
    for (int i = 0; i < numVertices; ++i) {
        Edge* e = vertices[i].edges;
        while (e) { Edge* nxt = e->next; delete e; e = nxt; }
    }
    delete[] vertices;
}

int Graph::indexOf(const std::string& name) const {
    for (int i = 0; i < numVertices; ++i) if (vertices[i].name == name) return i;
    return -1;
}

bool Graph::addLocation(const std::string&) { /* TODO */ return false; }
bool Graph::addConnection(const std::string&, const std::string&, int) { /* TODO */ return false; }
void Graph::displayConnections() const { /* TODO */ }
void Graph::bfs(const std::string&) const { /* TODO */ }
void Graph::dfs(const std::string&) const { /* TODO */ }
bool Graph::isReachable(const std::string&, const std::string&) const { /* TODO */ return false; }
void Graph::shortestPath(const std::string&, const std::string&) const { /* TODO */ }

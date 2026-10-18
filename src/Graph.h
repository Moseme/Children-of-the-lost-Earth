// OWNER: Student C. Campus locations (vertices) and pathways (edges).
// Adjacency list built from our own linked lists (no std::vector/list for core structure).
#ifndef GRAPH_H
#define GRAPH_H
#include <string>

class Graph {
private:
    struct Edge {
        int to;
        int weight;      // distance in metres, used by shortest path
        Edge* next;
        Edge(int t, int w, Edge* n) : to(t), weight(w), next(n) {}
    };
    struct Vertex {
        std::string name;
        Edge* edges;
        Vertex() : edges(NULL) {}
    };
    Vertex* vertices;
    int capacity;
    int numVertices;

    Graph(const Graph&);                       // copying is forbidden (declared, never written)
    Graph& operator=(const Graph&);

    int indexOf(const std::string& name) const;

public:
    explicit Graph(int maxVertices = 50);
    ~Graph();

    bool addLocation(const std::string& name);
    bool addConnection(const std::string& a, const std::string& b, int weight = 1); // undirected
    void displayConnections() const;
    void bfs(const std::string& start) const;
    void dfs(const std::string& start) const;
    bool isReachable(const std::string& a, const std::string& b) const;      // advanced feature option
    void shortestPath(const std::string& a, const std::string& b) const;    // advanced feature option
};

#endif

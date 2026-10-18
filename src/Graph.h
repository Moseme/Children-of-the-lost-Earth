// OWNER: Student C.
// A weighted, undirected GRAPH of campus locations.
//   vertex = a location (Library, CS Building, ...)
//   edge   = a walkway between two locations, with its length in metres (the "weight")
//
//   Library --120-- CS Building --80-- Building 32
//      |
//     150
//      |
//   Administration
//
// Representation: an ADJACENCY LIST. Each location keeps a linked list of the locations
// directly connected to it. Why not an adjacency matrix (report material)?
//   - a campus has few walkways per building, so the matrix would be mostly empty (V x V memory)
//   - the list uses O(V + E) memory and lets us visit just the real neighbours of a location
//
// Algorithms in this class:
//   BFS  (breadth-first search)    O(V + E)   finds the FEWEST-STEPS route
//   DFS  (depth-first search)      O(V + E)   goes as deep as possible before backing up
//   reachability                   O(V + E)
//   connected components           O(V + E)
//   shortest path (Dijkstra)       O(V^2)     finds the SHORTEST DISTANCE in metres
#ifndef GRAPH_H
#define GRAPH_H
#include <string>
#include <ostream>

class Graph {
private:
    struct Edge {
        int to;           // index of the neighbouring location
        int weight;       // walking distance in metres
        Edge* next;       // next neighbour in this location's list
        Edge(int t, int w) : to(t), weight(w), next(NULL) {}
    };
    struct Vertex {
        std::string name;
        Edge* head;       // first neighbour (NULL if none)
        Vertex() : head(NULL) {}
    };

    Vertex* vertices;     // dynamic array of locations
    int capacity;         // how many locations the array can hold
    int numVertices;      // how many locations exist

    Graph(const Graph&);              // copying forbidden (declared, never written)
    Graph& operator=(const Graph&);

    void growVertices();
    bool hasEdge(int a, int b) const;
    void addEdgeOneWay(int from, int to, int weight);
    void dfsVisit(int v, bool visited[], int& order) const;
    void dijkstra(int source, int dist[], int prev[]) const;

public:
    explicit Graph(int initialCapacity = 16);
    ~Graph();

    int indexOf(const std::string& name) const;       // -1 if there is no such location
    int vertexCount() const { return numVertices; }
    const std::string& nameAt(int index) const { return vertices[index].name; }

    bool addLocation(const std::string& name);        // false if it already exists
    // Connects two EXISTING locations in both directions. weight = metres (> 0).
    // false if a location is missing, a == b, the weight is bad, or they are already connected.
    bool addConnection(const std::string& a, const std::string& b, int weight);

    void listLocationNames() const;                   // one line: all location names
    void displayConnections() const;                  // every location and its neighbours

    void bfs(const std::string& start) const;
    void dfs(const std::string& start) const;
    bool isReachable(const std::string& from, const std::string& to) const;
    bool shortestPath(const std::string& from, const std::string& to) const;   // prints the route
    void displayComponents() const;

    // Distance in metres from 'start' to EVERY location (index = location index, -1 = unreachable).
    // n receives the number of locations. Caller must delete[] the result. NULL if start is unknown.
    int* distancesFrom(const std::string& start, int& n) const;

    // Writes the graph in the locations.txt format (isolated places as "name", links as "a|b|metres").
    void writeEdges(std::ostream& out) const;
};

#endif

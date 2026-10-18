#include "Graph.h"
#include <iostream>

static const int INF = 2000000000;     // "infinitely far": larger than any real distance

Graph::Graph(int initialCapacity) : vertices(NULL), capacity(initialCapacity), numVertices(0) {
    if (capacity < 1) capacity = 1;
    vertices = new Vertex[capacity];
}

Graph::~Graph() {
    for (int i = 0; i < numVertices; i++) {
        Edge* current = vertices[i].head;
        while (current != NULL) {
            Edge* nextEdge = current->next;
            delete current;
            current = nextEdge;
        }
    }
    delete[] vertices;
}

int Graph::indexOf(const std::string& name) const {
    for (int i = 0; i < numVertices; i++) {
        if (vertices[i].name == name) return i;
    }
    return -1;
}

// When the array is full, make one twice as big and move the locations across.
void Graph::growVertices() {
    int newCapacity = capacity * 2;
    Vertex* bigger = new Vertex[newCapacity];
    for (int i = 0; i < numVertices; i++) {
        bigger[i].name = vertices[i].name;
        bigger[i].head = vertices[i].head;     // the edge lists are simply handed over
    }
    delete[] vertices;
    vertices = bigger;
    capacity = newCapacity;
}

bool Graph::addLocation(const std::string& name) {
    if (name.empty() || indexOf(name) != -1) {
        return false;
    }
    if (numVertices == capacity) {
        growVertices();
    }
    vertices[numVertices].name = name;
    vertices[numVertices].head = NULL;
    numVertices++;
    return true;
}

bool Graph::hasEdge(int a, int b) const {
    for (Edge* e = vertices[a].head; e != NULL; e = e->next) {
        if (e->to == b) return true;
    }
    return false;
}

// Adds the edge at the END of the list, so neighbours are visited in the order they were added.
void Graph::addEdgeOneWay(int from, int to, int weight) {
    Edge* newEdge = new Edge(to, weight);
    if (vertices[from].head == NULL) {
        vertices[from].head = newEdge;
    } else {
        Edge* current = vertices[from].head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newEdge;
    }
}

bool Graph::addConnection(const std::string& a, const std::string& b, int weight) {
    int i = indexOf(a);
    int j = indexOf(b);
    if (i == -1 || j == -1 || i == j || weight <= 0 || weight > 100000) {
        return false;
    }
    if (hasEdge(i, j)) {
        return false;
    }
    addEdgeOneWay(i, j, weight);     // undirected: store the walkway in both directions
    addEdgeOneWay(j, i, weight);
    return true;
}

void Graph::listLocationNames() const {
    if (numVertices == 0) {
        std::cout << "(no locations yet)\n";
        return;
    }
    for (int i = 0; i < numVertices; i++) {
        std::cout << vertices[i].name;
        if (i + 1 < numVertices) std::cout << ", ";
    }
    std::cout << "\n";
}

void Graph::displayConnections() const {
    if (numVertices == 0) {
        std::cout << "(the campus graph is empty)\n";
        return;
    }
    for (int i = 0; i < numVertices; i++) {
        std::cout << vertices[i].name << " -> ";
        if (vertices[i].head == NULL) {
            std::cout << "(no connections)";
        }
        for (Edge* e = vertices[i].head; e != NULL; e = e->next) {
            std::cout << vertices[e->to].name << " (" << e->weight << " m)";
            if (e->next != NULL) std::cout << ", ";
        }
        std::cout << "\n";
    }
}

// ---------------------------------------------------------------- BFS
// Breadth-first search uses a QUEUE: visit a location, then add all its unvisited neighbours
// to the back of the queue. Locations are therefore reached in order of how many walkways away
// they are: all the 1-step neighbours first, then the 2-step ones, and so on.
void Graph::bfs(const std::string& start) const {
    int s = indexOf(start);
    if (s == -1) {
        std::cout << "Unknown location '" << start << "'.\n";
        return;
    }
    bool* visited = new bool[numVertices];
    int* steps = new int[numVertices];
    int* queue = new int[numVertices];
    for (int i = 0; i < numVertices; i++) visited[i] = false;
    int head = 0;
    int tail = 0;
    visited[s] = true;
    steps[s] = 0;
    queue[tail++] = s;
    int order = 0;
    while (head < tail) {
        int v = queue[head++];                       // dequeue
        order++;
        std::cout << "  " << order << ". " << vertices[v].name << "  (" << steps[v] << " walkway"
                  << (steps[v] == 1 ? "" : "s") << " from " << start << ")\n";
        for (Edge* e = vertices[v].head; e != NULL; e = e->next) {
            if (!visited[e->to]) {
                visited[e->to] = true;               // mark when ADDED, so nothing is queued twice
                steps[e->to] = steps[v] + 1;
                queue[tail++] = e->to;               // enqueue
            }
        }
    }
    std::cout << "BFS reached " << order << " of " << numVertices << " locations.\n";
    delete[] visited;
    delete[] steps;
    delete[] queue;
}

// ---------------------------------------------------------------- DFS
// Depth-first search follows one path as far as it can, then backs up and tries the next.
// The recursion itself acts as the STACK of locations still to be finished.
void Graph::dfsVisit(int v, bool visited[], int& order) const {
    visited[v] = true;
    order++;
    std::cout << "  " << order << ". " << vertices[v].name << "\n";
    for (Edge* e = vertices[v].head; e != NULL; e = e->next) {
        if (!visited[e->to]) {
            dfsVisit(e->to, visited, order);
        }
    }
}

void Graph::dfs(const std::string& start) const {
    int s = indexOf(start);
    if (s == -1) {
        std::cout << "Unknown location '" << start << "'.\n";
        return;
    }
    bool* visited = new bool[numVertices];
    for (int i = 0; i < numVertices; i++) visited[i] = false;
    int order = 0;
    dfsVisit(s, visited, order);
    std::cout << "DFS reached " << order << " of " << numVertices << " locations.\n";
    delete[] visited;
}

// ---------------------------------------------------------------- reachability and components
bool Graph::isReachable(const std::string& from, const std::string& to) const {
    int s = indexOf(from);
    int t = indexOf(to);
    if (s == -1 || t == -1) {
        return false;
    }
    bool* visited = new bool[numVertices];
    int* queue = new int[numVertices];
    for (int i = 0; i < numVertices; i++) visited[i] = false;
    int head = 0;
    int tail = 0;
    visited[s] = true;
    queue[tail++] = s;
    while (head < tail && !visited[t]) {
        int v = queue[head++];
        for (Edge* e = vertices[v].head; e != NULL; e = e->next) {
            if (!visited[e->to]) {
                visited[e->to] = true;
                queue[tail++] = e->to;
            }
        }
    }
    bool found = visited[t];
    delete[] visited;
    delete[] queue;
    return found;
}

// A connected component is a group of locations that can all reach each other.
// Run a BFS from every location not yet labelled; each BFS labels one whole component.
void Graph::displayComponents() const {
    if (numVertices == 0) {
        std::cout << "(the campus graph is empty)\n";
        return;
    }
    int* label = new int[numVertices];
    int* queue = new int[numVertices];
    for (int i = 0; i < numVertices; i++) label[i] = 0;      // 0 = not labelled yet
    int components = 0;
    for (int start = 0; start < numVertices; start++) {
        if (label[start] != 0) continue;
        components++;
        int head = 0;
        int tail = 0;
        label[start] = components;
        queue[tail++] = start;
        while (head < tail) {
            int v = queue[head++];
            for (Edge* e = vertices[v].head; e != NULL; e = e->next) {
                if (label[e->to] == 0) {
                    label[e->to] = components;
                    queue[tail++] = e->to;
                }
            }
        }
    }
    std::cout << components << " connected component" << (components == 1 ? "" : "s") << ":\n";
    for (int c = 1; c <= components; c++) {
        std::cout << "  Group " << c << ": ";
        bool first = true;
        for (int i = 0; i < numVertices; i++) {
            if (label[i] == c) {
                if (!first) std::cout << ", ";
                std::cout << vertices[i].name;
                first = false;
            }
        }
        std::cout << "\n";
    }
    delete[] label;
    delete[] queue;
}

// ---------------------------------------------------------------- shortest path (Dijkstra)
// Dijkstra's algorithm: keep the best known distance to every location. Repeatedly take the
// closest location not yet finished, and see whether going THROUGH it gives a shorter route to
// its neighbours. This version scans an array for the closest location, so it is O(V^2).
void Graph::dijkstra(int source, int dist[], int prev[]) const {
    bool* done = new bool[numVertices];
    for (int i = 0; i < numVertices; i++) {
        dist[i] = INF;
        prev[i] = -1;                  // which location we came from (to rebuild the route)
        done[i] = false;
    }
    dist[source] = 0;
    for (int round = 0; round < numVertices; round++) {
        int u = -1;
        for (int i = 0; i < numVertices; i++) {
            if (!done[i] && dist[i] < INF && (u == -1 || dist[i] < dist[u])) {
                u = i;
            }
        }
        if (u == -1) break;            // everything still unfinished is unreachable
        done[u] = true;
        for (Edge* e = vertices[u].head; e != NULL; e = e->next) {
            if (!done[e->to] && dist[u] + e->weight < dist[e->to]) {
                dist[e->to] = dist[u] + e->weight;
                prev[e->to] = u;
            }
        }
    }
    delete[] done;
}

bool Graph::shortestPath(const std::string& from, const std::string& to) const {
    int s = indexOf(from);
    int t = indexOf(to);
    if (s == -1 || t == -1) {
        std::cout << "Unknown location.\n";
        return false;
    }
    int* dist = new int[numVertices];
    int* prev = new int[numVertices];
    dijkstra(s, dist, prev);
    bool found = (dist[t] < INF);
    if (!found) {
        std::cout << "There is no route from " << from << " to " << to << ".\n";
    } else {
        int* path = new int[numVertices];
        int length = 0;
        for (int v = t; v != -1; v = prev[v]) {      // follow 'prev' backwards from the target...
            path[length++] = v;
        }
        std::cout << "Shortest route: ";
        for (int i = length - 1; i >= 0; i--) {      // ...and print it forwards
            std::cout << vertices[path[i]].name;
            if (i > 0) std::cout << " -> ";
        }
        std::cout << "\nTotal distance: " << dist[t] << " m\n";
        delete[] path;
    }
    delete[] dist;
    delete[] prev;
    return found;
}

int* Graph::distancesFrom(const std::string& start, int& n) const {
    n = numVertices;
    int s = indexOf(start);
    if (s == -1) {
        return NULL;
    }
    int* dist = new int[numVertices];
    int* prev = new int[numVertices];
    dijkstra(s, dist, prev);
    for (int i = 0; i < numVertices; i++) {
        if (dist[i] >= INF) dist[i] = -1;     // -1 means unreachable
    }
    delete[] prev;
    return dist;
}

void Graph::writeEdges(std::ostream& out) const {
    for (int i = 0; i < numVertices; i++) {
        if (vertices[i].head == NULL) {
            out << vertices[i].name << "\n";                    // a location with no walkways
        }
        for (Edge* e = vertices[i].head; e != NULL; e = e->next) {
            if (i < e->to) {                                    // write each walkway only once
                out << vertices[i].name << "|" << vertices[e->to].name << "|" << e->weight << "\n";
            }
        }
    }
}

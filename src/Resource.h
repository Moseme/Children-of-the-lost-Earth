// SHARED: agree any change to this file with all three members.
#ifndef RESOURCE_H
#define RESOURCE_H
#include <string>
#include <cstddef>

struct Resource {
    std::string id;        // e.g. LAB101 (unique key used by BST and hash table)
    std::string name;      // e.g. ICT Lab
    std::string type;      // e.g. Computer Laboratory
    std::string location;  // must match a vertex name in the campus Graph
    int capacity;
    bool available;

    Resource() : capacity(0), available(true) {}
    Resource(std::string i, std::string n, std::string t, std::string loc, int cap, bool avail)
        : id(i), name(n), type(t), location(loc), capacity(cap), available(avail) {}
};

#endif

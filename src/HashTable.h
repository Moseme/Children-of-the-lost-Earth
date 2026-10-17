// OWNER: Student C. Maps Resource ID -> Resource using separate chaining.
#ifndef HASHTABLE_H
#define HASHTABLE_H
#include <string>
#include <iostream>
#include "Resource.h"

class HashTable {
private:
    struct Node {
        Resource data;
        Node* next;
        Node(const Resource& r) : data(r), next(nullptr) {}
    };
    Node** buckets;
    int tableSize;
    int count;

    int hash(const std::string& key) const; // TODO: choose and justify hash function

public:
    explicit HashTable(int size = 31);      // TODO: confirm table size with lecturer parameters
    ~HashTable();
    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;

    bool insert(const Resource& r);         // false if duplicate ID
    Resource* find(const std::string& id);  // nullptr if missing
    bool remove(const std::string& id);
    int size() const { return count; }
    void display() const;
};

#endif

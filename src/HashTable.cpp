#include "HashTable.h"
#include <iostream>

HashTable::HashTable(int size) : buckets(NULL), tableSize(size), count(0) {
    if (tableSize < 1) tableSize = 1;
    buckets = new Node*[tableSize];
    for (int i = 0; i < tableSize; i++) {
        buckets[i] = NULL;
    }
}

HashTable::~HashTable() {
    for (int i = 0; i < tableSize; i++) {
        Node* current = buckets[i];
        while (current != NULL) {
            Node* nextNode = current->next;
            delete current;                  // frees the entry only, never the Resource itself
            current = nextNode;
        }
    }
    delete[] buckets;
}

// djb2 hash: start at 5381 and, for each character, multiply by 33 and add the character.
// (unsigned arithmetic simply wraps around when the number gets too big, which is fine for hashing.)
unsigned long HashTable::hashValue(const std::string& key) const {
    unsigned long h = 5381;
    for (int i = 0; i < (int)key.size(); i++) {
        h = h * 33 + (unsigned char)key[i];
    }
    return h;
}

int HashTable::indexFor(const std::string& key, int size) const {
    return (int)(hashValue(key) % (unsigned long)size);   // modulo maps the number onto 0 .. size-1
}

double HashTable::loadFactor() const {
    return (double)count / (double)tableSize;
}

// Build a bigger bucket array and move every entry into it. Entries are re-hashed because
// "hash % size" gives different answers when the size changes. Cost O(n), but it is rare.
void HashTable::rehash(int newSize) {
    Node** newBuckets = new Node*[newSize];
    for (int i = 0; i < newSize; i++) {
        newBuckets[i] = NULL;
    }
    for (int i = 0; i < tableSize; i++) {
        Node* current = buckets[i];
        while (current != NULL) {
            Node* nextNode = current->next;
            int index = indexFor(current->data->id, newSize);
            current->next = newBuckets[index];      // move the node to the front of its new chain
            newBuckets[index] = current;
            current = nextNode;
        }
    }
    delete[] buckets;
    buckets = newBuckets;
    tableSize = newSize;
}

bool HashTable::insert(Resource* r) {
    if (r == NULL) {
        return false;
    }
    if (find(r->id) != NULL) {
        return false;                               // duplicate ID
    }
    if (count + 1 > tableSize) {
        rehash(tableSize * 2 + 1);                  // keep the load factor at 1 or below
    }
    int index = indexFor(r->id, tableSize);
    Node* newNode = new Node(r);
    newNode->next = buckets[index];                 // put it at the FRONT of the chain: O(1)
    buckets[index] = newNode;
    count++;
    return true;
}

Resource* HashTable::find(const std::string& id, long& comparisons) const {
    int index = indexFor(id, tableSize);
    Node* current = buckets[index];
    while (current != NULL) {                       // walk the chain of this one bucket only
        comparisons++;
        if (current->data->id == id) {
            return current->data;
        }
        current = current->next;
    }
    return NULL;
}

Resource* HashTable::find(const std::string& id) const {
    long ignored = 0;
    return find(id, ignored);
}

bool HashTable::remove(const std::string& id) {
    int index = indexFor(id, tableSize);
    Node* previous = NULL;
    Node* current = buckets[index];
    while (current != NULL) {
        if (current->data->id == id) {
            if (previous == NULL) {
                buckets[index] = current->next;     // it was the first entry in the chain
            } else {
                previous->next = current->next;     // skip over it
            }
            delete current;
            count--;
            return true;
        }
        previous = current;
        current = current->next;
    }
    return false;
}

int HashTable::usedBuckets() const {
    int used = 0;
    for (int i = 0; i < tableSize; i++) {
        if (buckets[i] != NULL) used++;
    }
    return used;
}

int HashTable::longestChain() const {
    int longest = 0;
    for (int i = 0; i < tableSize; i++) {
        int length = 0;
        for (Node* current = buckets[i]; current != NULL; current = current->next) {
            length++;
        }
        if (length > longest) longest = length;
    }
    return longest;
}

void HashTable::display(int maxBuckets) const {
    if (count == 0) {
        std::cout << "(the hash table is empty)\n";
        return;
    }
    int shown = 0;
    for (int i = 0; i < tableSize; i++) {
        if (buckets[i] == NULL) continue;
        if (shown >= maxBuckets) {
            std::cout << "... (more buckets not shown)\n";
            return;
        }
        std::cout << "Bucket " << i << ": ";
        for (Node* current = buckets[i]; current != NULL; current = current->next) {
            std::cout << "[" << current->data->id << "]";
            if (current->next != NULL) std::cout << " -> ";
        }
        std::cout << "\n";
        shown++;
    }
}

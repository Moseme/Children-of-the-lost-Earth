#include "HashTable.h"

HashTable::HashTable(int size) : tableSize(size), count(0) {
    buckets = new Node*[tableSize];
    for (int i = 0; i < tableSize; ++i) buckets[i] = NULL;
}

HashTable::~HashTable() {
    for (int i = 0; i < tableSize; ++i) {
        Node* cur = buckets[i];
        while (cur) { Node* nxt = cur->next; delete cur; cur = nxt; }
    }
    delete[] buckets;
}

int HashTable::hash(const std::string& key) const {
    // TODO: replace with the group's chosen hash function (polynomial rolling hash suggested)
    unsigned long h = 0;
    for (int i = 0; i < (int)key.size(); i++) h = h * 31 + (unsigned char)key[i];
    return static_cast<int>(h % tableSize);
}

bool HashTable::insert(const Resource&) { /* TODO */ return false; }
Resource* HashTable::find(const std::string&) { /* TODO */ return NULL; }
bool HashTable::remove(const std::string&) { /* TODO */ return false; }
void HashTable::display() const { /* TODO */ }

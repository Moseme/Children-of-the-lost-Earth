// OWNER: Student C.
// A HASH TABLE that finds a resource from its ID in (average) O(1) time.
//
// The table is an array of "buckets". A hash FUNCTION turns the ID text into a number,
// and that number (modulo the table size) says which bucket the resource belongs in.
//
//   "LAB101" --hash function--> 3        bucket 3: [LAB101] -> [EQP202]   <-- a collision:
//   "EQP202" --hash function--> 3        bucket 7: [LAB104]                    two IDs, same bucket
//
// Collision handling: SEPARATE CHAINING. Every bucket holds a small linked list of the
// resources that landed in it. Why we chose it (report material):
//   + the table can never be "full" (a chain just gets longer)
//   + deleting is simple (unlink one node), unlike linear probing which needs tombstones
//   + one bad hash value does not spoil neighbouring buckets (no clustering)
//   - needs an extra pointer per entry
// To keep the chains short, the table doubles (and re-hashes) when it holds more entries than buckets.
//
// IMPORTANT DESIGN CHOICE: the table stores POINTERS to the resources inside Student A's linked
// list, not copies. So there is only ONE copy of each resource, and when it is updated
// (or approved by a request) the table sees the change automatically.
#ifndef HASHTABLE_H
#define HASHTABLE_H
#include <string>
#include "Resource.h"

class HashTable {
private:
    struct Node {
        Resource* data;     // points at the resource stored in the master list
        Node* next;         // next entry in the same bucket
        Node(Resource* r) : data(r), next(NULL) {}
    };

    Node** buckets;         // dynamic array of chain heads
    int tableSize;          // number of buckets
    int count;              // number of entries stored

    HashTable(const HashTable&);               // copying forbidden (declared, never written)
    HashTable& operator=(const HashTable&);

    int indexFor(const std::string& key, int size) const;   // bucket number for a key
    void rehash(int newSize);                                // rebuild with more buckets

public:
    explicit HashTable(int size = 53);         // a PRIME size spreads keys best
    ~HashTable();

    // The hash function itself (djb2): uses every character, so similar IDs like
    // LAB101 and LAB102 still land in different buckets.
    unsigned long hashValue(const std::string& key) const;
    int bucketOf(const std::string& key) const { return indexFor(key, tableSize); }

    bool insert(Resource* r);                              // false if NULL or the ID already exists
    Resource* find(const std::string& id) const;           // NULL if not found
    Resource* find(const std::string& id, long& comparisons) const;   // also counts ID comparisons
    bool remove(const std::string& id);                    // removes the entry (not the resource itself)

    int size() const { return count; }
    int bucketCount() const { return tableSize; }
    double loadFactor() const;                 // entries / buckets
    int usedBuckets() const;                   // buckets that hold at least one entry
    int longestChain() const;                  // the worst bucket: how slow a lookup can get
    void display(int maxBuckets) const;        // prints the non-empty buckets and their chains
};

#endif

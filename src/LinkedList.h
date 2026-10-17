// OWNER: Student A.
// A hand-written SINGLY LINKED LIST that stores Resource objects.
//
//   head --> [R1|next] --> [R2|next] --> [R3|next] --> nullptr
//                                          ^
//                                         tail
//
// Why a linked list instead of a fixed-size array here?
//  - The number of resources changes all the time (add/remove), and a list grows
//    and shrinks one node at a time. A fixed array needs a maximum size chosen in advance.
//  - Inserting at the beginning or removing a node needs no shifting of other items.
//  - The trade-off: no direct access by index (we must walk the list), so searching is O(n).
//
// Positions are numbered from 1 (the first resource is position 1).
#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include <string>
#include "Resource.h"

struct ListNode {
    Resource data;
    ListNode* next;
    ListNode(const Resource& r) : data(r), next(nullptr) {}
};

class LinkedList {
private:
    ListNode* head;   // first node (nullptr when the list is empty)
    ListNode* tail;   // last node, kept so that insertion at the end is O(1)
    int count;        // number of nodes

public:
    LinkedList();
    ~LinkedList();                                  // frees every node
    LinkedList(const LinkedList&) = delete;         // copying would share nodes, so forbid it
    LinkedList& operator=(const LinkedList&) = delete;

    // ----- insertion -----
    void insertAtBeginning(const Resource& r);              // O(1)
    void insertAtEnd(const Resource& r);                    // O(1) because of tail
    bool insertAtPosition(int position, const Resource& r); // O(n); valid positions: 1 .. size+1

    // ----- deletion -----
    // Removes the resource with this ID. The removed data is copied into 'removed'
    // (useful later for the UNDO stack). Returns false if the ID is not found. O(n)
    bool removeById(const std::string& id, Resource& removed);

    // ----- searching (linear search) -----
    Resource* findById(const std::string& id) const;        // O(n), nullptr if not found
    Resource* findByName(const std::string& name) const;    // O(n), first match

    // ----- other -----
    void display() const;                  // traversal: prints every resource, O(n)
    int size() const { return count; }
    bool isEmpty() const { return count == 0; }
    void clear();                          // removes every node

    // Copies the list into a NEW dynamic array (so we can sort it / binary-search it).
    // n receives the number of items. The caller MUST free it with delete[].
    // Returns nullptr when the list is empty.
    Resource* toArray(int& n) const;
};

#endif

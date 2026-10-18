// OWNER: Student B.
// A PRIORITY QUEUE implemented with a BINARY MIN-HEAP stored in an array.
//
// Priority queue = the ABSTRACT idea ("always give me the most urgent item first").
// Heap           = the DATA STRUCTURE we use to do it efficiently.
//
// A heap is a complete binary tree stored in an array. For the item at index i:
//     parent      is at (i - 1) / 2
//     left child  is at 2 * i + 1
//     right child is at 2 * i + 2
// Heap rule: every parent comes BEFORE (is more urgent than) both of its children,
// so the most urgent request is always at index 0.
//
//   insert           : put at the end, then swap UPWARD while more urgent than its parent.  O(log n)
//   removeHighest    : take index 0, move the last item there, then swap DOWNWARD.          O(log n)
//   peekHighest      : look at index 0.                                                     O(1)
//
// "More urgent" is decided by requestComesBefore(): priority number, then time, then ID,
// so requests with the same priority are still served first-come-first-served.
#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H
#include <string>
#include "Request.h"

class PriorityQueue {
private:
    Request* heap;      // dynamic array holding the heap
    int capacity;       // how many requests the array can hold right now
    int count;          // how many requests are stored

    PriorityQueue(const PriorityQueue&);               // copying forbidden (declared, never written)
    PriorityQueue& operator=(const PriorityQueue&);

    void swapRequests(int i, int j);
    void siftUp(int index);       // move an item up until the heap rule holds
    void siftDown(int index);     // move an item down until the heap rule holds
    void grow();                  // double the array size when it is full

public:
    PriorityQueue();
    ~PriorityQueue();

    void insert(const Request& r);
    bool removeHighest(Request& out);          // false if empty
    const Request* peekHighest() const;        // NULL if empty
    bool isEmpty() const { return count == 0; }
    int size() const { return count; }
    bool containsId(const std::string& id) const;   // O(n)

    void displayInPriorityOrder() const;       // the order requests WILL be processed
    void displayHeapArray() const;             // the raw array, to show how the heap is stored
    void clear();

    // Copy into a NEW array (heap order, NOT sorted). Caller must delete[] it. NULL if empty.
    Request* toArray(int& n) const;
};

#endif

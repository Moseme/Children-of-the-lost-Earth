// OWNER: Student B.
// A FIFO QUEUE of requests, built from linked nodes.
//
//   front --> [REQ1] --> [REQ2] --> [REQ3] <-- rear
//   (dequeue here)                  (enqueue here)
//
// FIFO = First In, First Out: the request that waited longest is served first.
// Both enqueue and dequeue are O(1) because we keep a pointer to each end.
#ifndef QUEUE_H
#define QUEUE_H
#include <string>
#include "Request.h"

struct QueueNode {
    Request data;
    QueueNode* next;
    QueueNode(const Request& r) : data(r), next(NULL) {}
};

class RequestQueue {
private:
    QueueNode* front;   // oldest request (leaves first)
    QueueNode* rear;    // newest request (joined last)
    int count;

    RequestQueue(const RequestQueue&);               // copying forbidden (declared, never written)
    RequestQueue& operator=(const RequestQueue&);

public:
    RequestQueue();
    ~RequestQueue();

    void enqueue(const Request& r);                  // add at the rear,       O(1)
    bool dequeue(Request& out);                      // remove from the front, O(1). false if empty
    const Request* peekFront() const;                // look at the front without removing. NULL if empty
    bool isEmpty() const { return count == 0; }
    int size() const { return count; }
    bool containsId(const std::string& id) const;    // O(n)
    void display() const;                            // traversal from front to rear, O(n)
    void clear();

    // Copy into a NEW array (front first). Caller must delete[] it. NULL if empty.
    Request* toArray(int& n) const;
};

#endif

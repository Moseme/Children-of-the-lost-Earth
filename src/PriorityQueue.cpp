#include "PriorityQueue.h"
#include "Utils.h"
#include <iostream>
#include <iomanip>

PriorityQueue::PriorityQueue() : heap(NULL), capacity(8), count(0) {
    heap = new Request[capacity];
}

PriorityQueue::~PriorityQueue() {
    delete[] heap;
}

void PriorityQueue::clear() {
    count = 0;      // the array is reused, so we only reset the counter
}

void PriorityQueue::swapRequests(int i, int j) {
    Request temp = heap[i];
    heap[i] = heap[j];
    heap[j] = temp;
}

void PriorityQueue::grow() {
    int newCapacity = capacity * 2;
    Request* bigger = new Request[newCapacity];
    for (int i = 0; i < count; i++) {
        bigger[i] = heap[i];
    }
    delete[] heap;
    heap = bigger;
    capacity = newCapacity;
}

void PriorityQueue::siftUp(int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (requestComesBefore(heap[index], heap[parent])) {
            swapRequests(index, parent);     // child is more urgent than its parent: swap
            index = parent;
        } else {
            break;                           // heap rule holds
        }
    }
}

void PriorityQueue::siftDown(int index) {
    while (true) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int best = index;                    // index of the most urgent among parent and children
        if (left < count && requestComesBefore(heap[left], heap[best])) {
            best = left;
        }
        if (right < count && requestComesBefore(heap[right], heap[best])) {
            best = right;
        }
        if (best == index) {
            break;                           // parent is already more urgent than both children
        }
        swapRequests(index, best);
        index = best;
    }
}

void PriorityQueue::insert(const Request& r) {
    if (count == capacity) {
        grow();
    }
    heap[count] = r;         // put the new request at the end...
    siftUp(count);           // ...then move it up to its correct place
    count++;
}

bool PriorityQueue::removeHighest(Request& out) {
    if (count == 0) {
        return false;
    }
    out = heap[0];                   // the most urgent request is always at the top
    heap[0] = heap[count - 1];       // move the last request to the top...
    count--;
    if (count > 0) {
        siftDown(0);                 // ...and let it sink to its correct place
    }
    return true;
}

const Request* PriorityQueue::peekHighest() const {
    if (count == 0) {
        return NULL;
    }
    return &heap[0];
}

bool PriorityQueue::containsId(const std::string& id) const {
    for (int i = 0; i < count; i++) {
        if (heap[i].id == id) return true;
    }
    return false;
}

// Shows requests in the order they will be processed WITHOUT disturbing the real heap:
// we copy everything into a temporary heap and remove from the copy one by one.
void PriorityQueue::displayInPriorityOrder() const {
    if (count == 0) {
        std::cout << "(no priority requests are waiting)\n";
        return;
    }
    PriorityQueue temp;
    for (int i = 0; i < count; i++) {
        temp.insert(heap[i]);
    }
    printRequestHeader();
    Request r;
    while (temp.removeHighest(r)) {
        printRequestRow(r);
    }
}

void PriorityQueue::displayHeapArray() const {
    if (count == 0) {
        std::cout << "(the heap is empty)\n";
        return;
    }
    std::cout << std::left
              << std::setw(7) << "Index"
              << std::setw(8) << "Parent"
              << std::setw(13) << "Priority"
              << std::setw(9) << "ID"
              << "Submitted\n";
    std::cout << std::string(50, '-') << "\n";
    for (int i = 0; i < count; i++) {
        std::string parentText = (i == 0) ? "-" : intToString((i - 1) / 2);
        std::string priorityText = intToString(heap[i].priority) + "-" + priorityName(heap[i].priority);
        std::cout << std::left
                  << std::setw(7) << i
                  << std::setw(8) << parentText
                  << std::setw(13) << priorityText
                  << std::setw(9) << heap[i].id
                  << heap[i].time << "\n";
    }
    std::cout << "(Index 0 is the root: always the most urgent request.)\n";
}

Request* PriorityQueue::toArray(int& n) const {
    n = count;
    if (count == 0) {
        return NULL;
    }
    Request* arr = new Request[count];
    for (int i = 0; i < count; i++) {
        arr[i] = heap[i];
    }
    return arr;
}

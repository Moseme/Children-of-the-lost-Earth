#include "Queue.h"
#include <iostream>

RequestQueue::RequestQueue() : front(NULL), rear(NULL), count(0) {
}

RequestQueue::~RequestQueue() {
    clear();
}

void RequestQueue::clear() {
    QueueNode* current = front;
    while (current != NULL) {
        QueueNode* nextNode = current->next;
        delete current;
        current = nextNode;
    }
    front = NULL;
    rear = NULL;
    count = 0;
}

void RequestQueue::enqueue(const Request& r) {
    QueueNode* newNode = new QueueNode(r);
    if (rear == NULL) {          // empty queue: the new node is both front and rear
        front = newNode;
        rear = newNode;
    } else {
        rear->next = newNode;    // old last node points to the new node
        rear = newNode;          // the new node is the new rear
    }
    count++;
}

bool RequestQueue::dequeue(Request& out) {
    if (front == NULL) {
        return false;            // queue underflow: nothing to remove
    }
    QueueNode* oldFront = front;
    out = oldFront->data;
    front = oldFront->next;
    if (front == NULL) {         // we removed the only node, so the queue is now empty
        rear = NULL;
    }
    delete oldFront;
    count--;
    return true;
}

const Request* RequestQueue::peekFront() const {
    if (front == NULL) {
        return NULL;
    }
    return &front->data;
}

bool RequestQueue::containsId(const std::string& id) const {
    QueueNode* current = front;
    while (current != NULL) {
        if (current->data.id == id) return true;
        current = current->next;
    }
    return false;
}

void RequestQueue::display() const {
    if (front == NULL) {
        std::cout << "(queue is empty)\n";
        return;
    }
    printRequestHeader();
    QueueNode* current = front;
    while (current != NULL) {
        printRequestRow(current->data);
        current = current->next;
    }
}

Request* RequestQueue::toArray(int& n) const {
    n = count;
    if (count == 0) {
        return NULL;
    }
    Request* arr = new Request[count];
    QueueNode* current = front;
    int i = 0;
    while (current != NULL) {
        arr[i] = current->data;
        i++;
        current = current->next;
    }
    return arr;
}

#include "LinkedList.h"
#include "Utils.h"
#include <iostream>

LinkedList::LinkedList() : head(nullptr), tail(nullptr), count(0) {
}

LinkedList::~LinkedList() {
    clear();
}

void LinkedList::clear() {
    ListNode* current = head;
    while (current != nullptr) {
        ListNode* nextNode = current->next;   // remember the next node BEFORE deleting this one
        delete current;
        current = nextNode;
    }
    head = nullptr;
    tail = nullptr;
    count = 0;
}

void LinkedList::insertAtBeginning(const Resource& r) {
    ListNode* newNode = new ListNode(r);
    newNode->next = head;       // new node points to the old first node
    head = newNode;             // head now points to the new node
    if (tail == nullptr) {      // list was empty, so the new node is also the last one
        tail = newNode;
    }
    count++;
}

void LinkedList::insertAtEnd(const Resource& r) {
    ListNode* newNode = new ListNode(r);
    if (head == nullptr) {      // empty list
        head = newNode;
        tail = newNode;
    } else {
        tail->next = newNode;   // old last node points to the new node
        tail = newNode;         // the new node is now the last one
    }
    count++;
}

bool LinkedList::insertAtPosition(int position, const Resource& r) {
    if (position < 1 || position > count + 1) {
        return false;                       // position does not exist
    }
    if (position == 1) {
        insertAtBeginning(r);
        return true;
    }
    if (position == count + 1) {
        insertAtEnd(r);
        return true;
    }
    // Walk to the node just BEFORE the wanted position.
    ListNode* previous = head;
    for (int i = 1; i < position - 1; i++) {
        previous = previous->next;
    }
    ListNode* newNode = new ListNode(r);
    newNode->next = previous->next;         // link the new node to the one after it
    previous->next = newNode;               // link the one before it to the new node
    count++;
    return true;
}

bool LinkedList::removeById(const std::string& id, Resource& removed) {
    ListNode* previous = nullptr;
    ListNode* current = head;
    while (current != nullptr) {
        if (current->data.id == id) {
            removed = current->data;                // keep a copy of what we delete
            if (previous == nullptr) {
                head = current->next;               // deleting the first node
            } else {
                previous->next = current->next;     // skip over the deleted node
            }
            if (current == tail) {
                tail = previous;                    // deleted the last node: tail moves back
            }
            delete current;
            count--;
            return true;
        }
        previous = current;
        current = current->next;
    }
    return false;   // reached the end without finding the ID
}

Resource* LinkedList::findById(const std::string& id) const {
    ListNode* current = head;
    while (current != nullptr) {
        if (current->data.id == id) {
            return &current->data;
        }
        current = current->next;
    }
    return nullptr;
}

Resource* LinkedList::findByName(const std::string& name) const {
    ListNode* current = head;
    while (current != nullptr) {
        if (current->data.name == name) {
            return &current->data;
        }
        current = current->next;
    }
    return nullptr;
}

void LinkedList::display() const {
    if (head == nullptr) {
        std::cout << "(no resources)\n";
        return;
    }
    printResourceHeader();
    ListNode* current = head;
    while (current != nullptr) {
        printResourceRow(current->data);
        current = current->next;
    }
}

Resource* LinkedList::toArray(int& n) const {
    n = count;
    if (count == 0) {
        return nullptr;
    }
    Resource* arr = new Resource[count];
    ListNode* current = head;
    int i = 0;
    while (current != nullptr) {
        arr[i] = current->data;
        i++;
        current = current->next;
    }
    return arr;
}

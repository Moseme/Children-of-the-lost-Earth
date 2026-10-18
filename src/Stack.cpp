#include "Stack.h"
#include "Utils.h"
#include <iostream>

UndoStack::UndoStack() : topNode(NULL), count(0) {
}

UndoStack::~UndoStack() {
    clear();
}

void UndoStack::clear() {
    StackNode* current = topNode;
    while (current != NULL) {
        StackNode* nextNode = current->next;
        delete current;
        current = nextNode;
    }
    topNode = NULL;
    count = 0;
}

void UndoStack::push(const UndoAction& action) {
    StackNode* newNode = new StackNode(action);
    newNode->next = topNode;      // the new node sits on top of the old top
    topNode = newNode;
    count++;
}

bool UndoStack::pop(UndoAction& out) {
    if (topNode == NULL) {
        return false;             // stack underflow: nothing to undo
    }
    StackNode* oldTop = topNode;
    out = oldTop->data;
    topNode = oldTop->next;
    delete oldTop;
    count--;
    return true;
}

const UndoAction* UndoStack::peek() const {
    if (topNode == NULL) {
        return NULL;
    }
    return &topNode->data;
}

void UndoStack::display() const {
    if (topNode == NULL) {
        std::cout << "(the undo stack is empty)\n";
        return;
    }
    StackNode* current = topNode;
    int number = 1;
    while (current != NULL) {
        std::cout << "  " << number << ". " << describeAction(current->data);
        if (number == 1) std::cout << "   <-- TOP (next to be undone)";
        std::cout << "\n";
        current = current->next;
        number++;
    }
}

std::string describeAction(const UndoAction& action) {
    const Resource& r = action.resource;
    if (action.type == UNDO_ADD) {
        return "Added " + r.id + " (" + r.name + ")";
    }
    if (action.type == UNDO_REMOVE) {
        return "Removed " + r.id + " (" + r.name + ") from position " + intToString(action.position);
    }
    if (action.type == UNDO_UPDATE) {
        return "Updated " + r.id + " (previous name: " + r.name + ")";
    }
    return "Unknown action";
}

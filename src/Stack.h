// OWNER: Student B.
// A STACK used for UNDO.
//
//        top --> [Update LAB101]     <- most recent action, undone first
//                [Remove LAB103]
//                [Add LAB200]        <- oldest action, undone last
//
// LIFO = Last In, First Out. Undo needs exactly this: the LAST thing we did is the
// FIRST thing we must reverse. push, pop and peek are all O(1).
//
// What is stored for each kind of action (the "resource" field):
//   UNDO_ADD    : the resource that was added          (undo = remove it again)
//   UNDO_REMOVE : the resource that was removed        (undo = put it back at 'position')
//   UNDO_UPDATE : the resource BEFORE it was changed   (undo = copy the old values back)
#ifndef STACK_H
#define STACK_H
#include <string>
#include "Resource.h"

enum UndoType {
    UNDO_ADD = 1,
    UNDO_REMOVE = 2,
    UNDO_UPDATE = 3
};

struct UndoAction {
    int type;
    Resource resource;
    int position;          // where a removed resource used to be (1 = first), used by UNDO_REMOVE
    UndoAction() : type(0), position(0) {}
};

struct StackNode {
    UndoAction data;
    StackNode* next;
    StackNode(const UndoAction& a) : data(a), next(NULL) {}
};

class UndoStack {
private:
    StackNode* topNode;
    int count;

    UndoStack(const UndoStack&);               // copying forbidden (declared, never written)
    UndoStack& operator=(const UndoStack&);

public:
    UndoStack();
    ~UndoStack();

    void push(const UndoAction& action);       // O(1)
    bool pop(UndoAction& out);                 // O(1). false if the stack is empty
    const UndoAction* peek() const;            // look at the top. NULL if the stack is empty
    bool isEmpty() const { return count == 0; }
    int size() const { return count; }
    void display() const;                      // prints from top (newest) to bottom (oldest)
    void clear();
};

// One-line description of an action, e.g. "Removed LAB103 (Hardware Repair Workshop)".
std::string describeAction(const UndoAction& action);

#endif

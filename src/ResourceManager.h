// OWNER: Student A (shared with the team as the "front door" to the resource data).
// Owns the master LinkedList of resources and provides the menus for
// main-menu options 1 (Resource Management), 2 (Search) and 3 (Sort),
// plus loading from and saving to a text file.
//
// File format, one resource per line:   id|name|type|location|capacity|available(1 or 0)
// Lines that are empty or start with '#' are ignored.
#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H
#include <string>
#include "LinkedList.h"
#include "Stack.h"
#include "HashTable.h"
#include "Tree.h"
#include "Resource.h"

class ResourceManager {
private:
    LinkedList list;     // the master store of all resources
    HashTable hashIndex; // finds a resource by ID in O(1)   (Student C)
    BST treeIndex;       // keeps resources ordered by ID     (Student C)
    UndoStack undoStack; // remembers add / remove / update so they can be undone (LIFO)

    // Keep the two indexes in step with the list. Both store POINTERS to the list's own data.
    void indexResource(Resource* stored);              // call right AFTER a resource joins the list
    void unindexResource(const std::string& id);       // call right BEFORE a resource leaves the list

    // --- Resource Management helpers ---
    void addResource(int where);         // where: 1 = beginning, 2 = end, 3 = chosen position
    void removeResource();
    void updateResource();
    void generateTestData();             // for the "large data" test case
    bool readResourceFields(Resource& r, bool askForId);
    bool isValidText(const std::string& text) const;   // not empty and has no '|'

    // --- Sort helpers ---
    void showSorted(int algorithm, int criterion);     // algorithm: 1 insertion, 2 merge
    void compareSortingAlgorithms(int criterion);

    // --- Undo helpers (Student B) ---
    void undoLast();
    void viewUndoTop() const;

    // --- Search helpers ---
    void searchByIdLinear();
    void searchByIdBinary();
    void searchByNameLinear();

public:
    // ----- menus (called from main) -----
    void resourceMenu();    // main menu option 1
    void searchMenu();      // main menu option 2
    void sortMenu();        // main menu option 3
    void undoMenu();        // main menu option 6 (Student B)

    // ----- file persistence -----
    bool loadFromFile(const std::string& filename);
    bool saveToFile(const std::string& filename) const;

    // ----- access for other modules (Student B and C) -----
    int count() const { return list.size(); }
    Resource* findResource(const std::string& id) const { return hashIndex.find(id); }   // O(1) average

    // Read-only access to the indexes (used by Student C's tree and hash menus).
    const HashTable& getHashIndex() const { return hashIndex; }
    const BST& getTreeIndex() const { return treeIndex; }
    // Copy of all resources in a new array. Caller must delete[] it. NULL if empty.
    Resource* getAllAsArray(int& n) const { return list.toArray(n); }

};

#endif

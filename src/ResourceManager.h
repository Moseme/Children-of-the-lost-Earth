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
#include "Resource.h"

class ResourceManager {
private:
    LinkedList list;     // the master store of all resources

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

    // --- Search helpers ---
    void searchByIdLinear();
    void searchByIdBinary();
    void searchByNameLinear();

public:
    // ----- menus (called from main) -----
    void resourceMenu();    // main menu option 1
    void searchMenu();      // main menu option 2
    void sortMenu();        // main menu option 3

    // ----- file persistence -----
    bool loadFromFile(const std::string& filename);
    bool saveToFile(const std::string& filename) const;

    // ----- access for other modules (Student B and C) -----
    int count() const { return list.size(); }
    Resource* findResource(const std::string& id) const { return list.findById(id); }
    // Copy of all resources in a new array. Caller must delete[] it. nullptr if empty.
    Resource* getAllAsArray(int& n) const { return list.toArray(n); }

    // Called by menus after a successful add/remove so other structures can be kept in step.
    // TODO (team): when Student C's BST and HashTable exist, insert/remove there too.
    // TODO (Student B): push the removed resource on the undo stack inside removeResource().
};

#endif

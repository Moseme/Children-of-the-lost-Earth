// OWNER: Student C.
// The menus for the advanced structures:
//   main menu option 7 : Tree Operations        (the BST of resources, ordered by ID)
//   main menu option 8 : Hash Table Lookup      (find a resource by ID, and compare lookup methods)
//   main menu option 9 : Campus Navigation      (the graph of locations and walkways)
// It also owns the campus Graph and saves/loads it from locations.txt.
//
// locations.txt format (one item per line, '#' starts a comment):
//   A|B|120      two locations joined by a walkway of 120 metres (the locations are created automatically)
//   Some Place   a location with no walkways yet
#ifndef CAMPUSMANAGER_H
#define CAMPUSMANAGER_H
#include <string>
#include "Graph.h"
#include "ResourceManager.h"

class CampusManager {
private:
    ResourceManager& resources;     // owns the hash table and BST we display, and the resource list
    Graph graph;

    CampusManager(const CampusManager&);               // copying forbidden
    CampusManager& operator=(const CampusManager&);

    // --- tree menu helpers ---
    void showTreeStatistics() const;
    void searchTree() const;

    // --- hash menu helpers ---
    void lookupById() const;
    void showHashStatistics() const;
    void showHashValue() const;
    void compareLookupMethods() const;

    // --- navigation helpers ---
    std::string askLocation(const std::string& prompt) const;   // "" if the name is unknown
    void addLocation();
    void addConnection();
    void runBfs() const;
    void runDfs() const;
    void checkReachability() const;
    void findShortestRoute() const;
    void findNearestResource() const;

public:
    explicit CampusManager(ResourceManager& rm);

    int locationCount() const { return graph.vertexCount(); }

    void treeMenu();            // main menu option 7
    void hashMenu();            // main menu option 8
    void navigationMenu();      // main menu option 9

    bool loadLocations(const std::string& filename);
    bool saveLocations(const std::string& filename) const;
};

#endif

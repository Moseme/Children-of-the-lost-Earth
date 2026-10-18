#include "CampusManager.h"
#include "Search.h"
#include "Sorting.h"
#include "Utils.h"
#include <iostream>
#include <iomanip>
#include <fstream>

// A location name must not be empty and must not contain '|' (the file separator).
static bool isValidLocationName(const std::string& text) {
    if (text.empty()) return false;
    for (int i = 0; i < (int)text.size(); i++) {
        if (text[i] == '|') return false;
    }
    return true;
}

CampusManager::CampusManager(ResourceManager& rm) : resources(rm) {
}

// =====================================================================
//  Module 7: Tree operations (menu option 7)
// =====================================================================

void CampusManager::showTreeStatistics() const {
    const BST& tree = resources.getTreeIndex();
    int n = tree.size();
    int minimumHeight = 0;
    while (((1 << minimumHeight) - 1) < n) {     // a perfectly full tree of h levels holds 2^h - 1 nodes
        minimumHeight++;
    }
    std::cout << "Resources in the tree : " << n << "\n"
              << "Height of the tree    : " << tree.height() << " levels\n"
              << "Smallest possible     : " << minimumHeight << " levels for " << n << " nodes\n";
    if (n > 0 && tree.height() > 2 * minimumHeight + 1) {
        std::cout << "The tree is very unbalanced (inserted in nearly sorted order), so searching\n"
                  << "behaves more like a linear search. This is the BST worst case, O(n).\n";
    } else if (n > 0) {
        std::cout << "The tree is reasonably balanced, so a search takes about O(log n) steps.\n";
    }
}

void CampusManager::searchTree() const {
    const BST& tree = resources.getTreeIndex();
    if (tree.empty()) {
        std::cout << "The tree is empty.\n";
        return;
    }
    std::string id = readLine("Resource ID to search for in the tree: ");
    long comparisons = 0;
    Resource* found = tree.search(id, comparisons);
    if (found != NULL) {
        std::cout << "Found:\n";
        printResourceHeader();
        printResourceRow(*found);
    } else {
        std::cout << "Resource '" << id << "' is not in the tree.\n";
    }
    std::cout << "Comparisons made: " << comparisons << " (the tree has " << tree.size() << " resources)\n";
}

void CampusManager::treeMenu() {
    const BST& tree = resources.getTreeIndex();
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n--- TREE OPERATIONS: BST of resources ordered by ID (" << tree.size() << " nodes) ---\n"
                  << "1. Inorder traversal (sorted by ID)\n"
                  << "2. Preorder traversal\n"
                  << "3. Postorder traversal\n"
                  << "4. Level-order traversal\n"
                  << "5. Draw the tree\n"
                  << "6. Search the tree by ID\n"
                  << "7. Tree statistics (size and height)\n"
                  << "0. Back to main menu\n"
                  << "(Resources are added to and deleted from the tree through Resource Management.)\n";
        choice = readInt("Enter your choice: ", 0, 7);
        if (choice == 1) tree.inorder(25);
        else if (choice == 2) tree.preorder(25);
        else if (choice == 3) tree.postorder(25);
        else if (choice == 4) tree.levelOrder(25);
        else if (choice == 5) tree.drawSideways(4);
        else if (choice == 6) searchTree();
        else if (choice == 7) showTreeStatistics();
    }
}

// =====================================================================
//  Module 8: Hash table lookup (menu option 8)
// =====================================================================

void CampusManager::lookupById() const {
    const HashTable& table = resources.getHashIndex();
    if (table.size() == 0) {
        std::cout << "The hash table is empty.\n";
        return;
    }
    std::string id = readLine("Resource ID to look up: ");
    long comparisons = 0;
    Resource* found = table.find(id, comparisons);
    std::cout << "Hash of '" << id << "' puts it in bucket " << table.bucketOf(id)
              << " (of " << table.bucketCount() << ").\n";
    if (found != NULL) {
        std::cout << "Found:\n";
        printResourceHeader();
        printResourceRow(*found);
    } else {
        std::cout << "Resource '" << id << "' was not found.\n";
    }
    std::cout << "IDs compared inside that bucket: " << comparisons << "\n";
}

void CampusManager::showHashStatistics() const {
    const HashTable& table = resources.getHashIndex();
    std::cout << "Entries stored   : " << table.size() << "\n"
              << "Buckets          : " << table.bucketCount() << "\n"
              << "Buckets in use   : " << table.usedBuckets() << "\n"
              << "Load factor      : " << std::fixed << std::setprecision(2) << table.loadFactor()
              << " (entries per bucket)\n";
    std::cout.unsetf(std::ios::fixed);
    std::cout << "Longest chain    : " << table.longestChain()
              << " (the most IDs any one lookup has to compare)\n"
              << "Collision method : separate chaining\n"
              << "Hash function    : djb2 (h = h * 33 + character), then h mod bucket count\n";
}

void CampusManager::showHashValue() const {
    const HashTable& table = resources.getHashIndex();
    std::string text = readLine("Text to hash (e.g. a resource ID): ");
    if (text.empty()) {
        std::cout << "Nothing to hash.\n";
        return;
    }
    std::cout << "hash value = " << table.hashValue(text) << "\n"
              << "bucket     = " << table.hashValue(text) << " mod " << table.bucketCount()
              << " = " << table.bucketOf(text) << "\n";
}

// Looks for the same ID with four different methods and shows how much work each one does.
void CampusManager::compareLookupMethods() const {
    int n;
    Resource* arr = resources.getAllAsArray(n);
    if (arr == NULL) {
        std::cout << "There are no resources to search.\n";
        return;
    }
    std::string id = readLine("Resource ID to look for: ");

    long linearComparisons = 0;
    bool linearFound = (linearSearchById(arr, n, id, linearComparisons) >= 0);

    long sortComparisons = 0;
    mergeSort(arr, n, SORT_BY_ID, sortComparisons);      // binary search needs sorted data
    long binaryComparisons = 0;
    bool binaryFound = (binarySearchById(arr, n, id, binaryComparisons) >= 0);

    long treeComparisons = 0;
    bool treeFound = (resources.getTreeIndex().search(id, treeComparisons) != NULL);
    long hashComparisons = 0;
    bool hashFound = (resources.getHashIndex().find(id, hashComparisons) != NULL);

    std::cout << "\nLooking for '" << id << "' among " << n << " resources:\n";
    std::cout << std::left << std::setw(34) << "Method" << std::setw(8) << "Found" << std::setw(13)
              << "Comparisons" << "Typical cost\n";
    std::cout << std::string(70, '-') << "\n";
    std::cout << std::left << std::setw(34) << "Linear search (unsorted list)" << std::setw(8)
              << (linearFound ? "yes" : "no") << std::setw(13) << linearComparisons << "O(n)\n";
    std::cout << std::left << std::setw(34) << "Binary search (sorted array)" << std::setw(8)
              << (binaryFound ? "yes" : "no") << std::setw(13) << binaryComparisons << "O(log n)\n";
    std::cout << std::left << std::setw(34) << "Binary search tree" << std::setw(8)
              << (treeFound ? "yes" : "no") << std::setw(13) << treeComparisons << "O(log n) average, O(n) worst\n";
    std::cout << std::left << std::setw(34) << "Hash table" << std::setw(8)
              << (hashFound ? "yes" : "no") << std::setw(13) << hashComparisons << "O(1) average\n";
    std::cout << "Note: binary search first needed a sorted copy (" << sortComparisons
              << " comparisons to sort it).\nThe tree and hash table are kept ready all the time, so they pay that cost only once.\n";
    delete[] arr;
}

void CampusManager::hashMenu() {
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n--- HASH TABLE LOOKUP: resource ID -> resource (" << resources.getHashIndex().size()
                  << " entries) ---\n"
                  << "1. Look up a resource by ID\n"
                  << "2. Show the hash table (buckets and chains)\n"
                  << "3. Hash table statistics\n"
                  << "4. Show the hash value of some text\n"
                  << "5. Compare lookup methods (linear, binary, tree, hash)\n"
                  << "0. Back to main menu\n";
        choice = readInt("Enter your choice: ", 0, 5);
        if (choice == 1) lookupById();
        else if (choice == 2) resources.getHashIndex().display(30);
        else if (choice == 3) showHashStatistics();
        else if (choice == 4) showHashValue();
        else if (choice == 5) compareLookupMethods();
    }
}

// =====================================================================
//  Module 9: Campus navigation (menu option 9)
// =====================================================================

// Asks for a location name. Returns "" (and explains) if it is not in the graph.
std::string CampusManager::askLocation(const std::string& prompt) const {
    std::string name = readLine(prompt);
    if (graph.indexOf(name) == -1) {
        std::cout << "Unknown location '" << name << "'. Known locations: ";
        graph.listLocationNames();
        return "";
    }
    return name;
}

void CampusManager::addLocation() {
    std::string name = readLine("Name of the new location: ");
    if (!isValidLocationName(name)) {
        std::cout << "A location name cannot be empty or contain '|'.\n";
    } else if (graph.addLocation(name)) {
        std::cout << "Location '" << name << "' added.\n";
    } else {
        std::cout << "A location named '" << name << "' already exists.\n";
    }
}

void CampusManager::addConnection() {
    std::cout << "Known locations: ";
    graph.listLocationNames();
    std::string a = askLocation("First location: ");
    if (a.empty()) return;
    std::string b = askLocation("Second location: ");
    if (b.empty()) return;
    if (a == b) {
        std::cout << "A location cannot be connected to itself.\n";
        return;
    }
    int metres = readInt("Walking distance in metres (1 - 100000): ", 1, 100000);
    if (graph.addConnection(a, b, metres)) {
        std::cout << a << " and " << b << " are now connected (" << metres << " m).\n";
    } else {
        std::cout << "They are already connected.\n";
    }
}

void CampusManager::runBfs() const {
    if (graph.vertexCount() == 0) { std::cout << "The campus graph is empty.\n"; return; }
    std::cout << "Known locations: ";
    graph.listLocationNames();
    std::string start = askLocation("Start location: ");
    if (start.empty()) return;
    std::cout << "Breadth-first search (nearest locations first, by number of walkways):\n";
    graph.bfs(start);
}

void CampusManager::runDfs() const {
    if (graph.vertexCount() == 0) { std::cout << "The campus graph is empty.\n"; return; }
    std::cout << "Known locations: ";
    graph.listLocationNames();
    std::string start = askLocation("Start location: ");
    if (start.empty()) return;
    std::cout << "Depth-first search (follows one path as deep as possible first):\n";
    graph.dfs(start);
}

void CampusManager::checkReachability() const {
    if (graph.vertexCount() == 0) { std::cout << "The campus graph is empty.\n"; return; }
    std::cout << "Known locations: ";
    graph.listLocationNames();
    std::string a = askLocation("From: ");
    if (a.empty()) return;
    std::string b = askLocation("To: ");
    if (b.empty()) return;
    if (graph.isReachable(a, b)) {
        std::cout << b << " IS reachable from " << a << ".\n";
    } else {
        std::cout << b << " is NOT reachable from " << a << " (no chain of walkways joins them).\n";
    }
}

void CampusManager::findShortestRoute() const {
    if (graph.vertexCount() == 0) { std::cout << "The campus graph is empty.\n"; return; }
    std::cout << "Known locations: ";
    graph.listLocationNames();
    std::string a = askLocation("From: ");
    if (a.empty()) return;
    std::string b = askLocation("To: ");
    if (b.empty()) return;
    graph.shortestPath(a, b);
}

// Combines the graph with the resource list: which AVAILABLE resource is closest to me?
void CampusManager::findNearestResource() const {
    if (graph.vertexCount() == 0) { std::cout << "The campus graph is empty.\n"; return; }
    std::cout << "Known locations: ";
    graph.listLocationNames();
    std::string from = askLocation("Your current location: ");
    if (from.empty()) return;
    std::string type = readLine("Resource type needed (press Enter for any type): ");

    int vertexCount;
    int* dist = graph.distancesFrom(from, vertexCount);     // distance to every location
    int n;
    Resource* all = resources.getAllAsArray(n);
    int best = -1;
    int bestDistance = -1;
    int notOnMap = 0;
    for (int i = 0; i < n; i++) {
        if (!all[i].available) continue;
        if (!type.empty() && all[i].type != type) continue;
        int v = graph.indexOf(all[i].location);
        if (v == -1) { notOnMap++; continue; }              // its location is not a campus location
        if (dist[v] < 0) continue;                          // cannot walk there from here
        if (best == -1 || dist[v] < bestDistance) {
            best = i;
            bestDistance = dist[v];
        }
    }
    if (best == -1) {
        std::cout << "No available resource" << (type.empty() ? "" : " of that type")
                  << " can be reached from " << from << ".\n";
    } else {
        std::cout << "Nearest available resource (" << bestDistance << " m away):\n";
        printResourceHeader();
        printResourceRow(all[best]);
        graph.shortestPath(from, all[best].location);
    }
    if (notOnMap > 0) {
        std::cout << "(" << notOnMap << " available resource(s) were skipped because their location is not on the campus map.)\n";
    }
    delete[] dist;
    delete[] all;
}

void CampusManager::navigationMenu() {
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n--- CAMPUS NAVIGATION (" << graph.vertexCount() << " locations) ---\n"
                  << "1. Add a location\n"
                  << "2. Add a connection (walkway) between two locations\n"
                  << "3. Display all locations and connections\n"
                  << "4. Breadth-first search (BFS) from a location\n"
                  << "5. Depth-first search (DFS) from a location\n"
                  << "6. Is a location reachable from another?\n"
                  << "7. Shortest route between two locations (Dijkstra)\n"
                  << "8. Find the nearest available resource\n"
                  << "9. Show connected groups of locations\n"
                  << "0. Back to main menu\n";
        choice = readInt("Enter your choice: ", 0, 9);
        if (choice == 1) addLocation();
        else if (choice == 2) addConnection();
        else if (choice == 3) graph.displayConnections();
        else if (choice == 4) runBfs();
        else if (choice == 5) runDfs();
        else if (choice == 6) checkReachability();
        else if (choice == 7) findShortestRoute();
        else if (choice == 8) findNearestResource();
        else if (choice == 9) graph.displayComponents();
    }
}

// =====================================================================
//  Data persistence
// =====================================================================

bool CampusManager::loadLocations(const std::string& filename) {
    std::ifstream in(filename.c_str());
    if (!in) {
        return false;
    }
    std::string line;
    int lineNumber = 0;
    int connections = 0;
    int skipped = 0;
    while (std::getline(in, line)) {
        lineNumber++;
        line = trim(line);
        if (line.empty() || line[0] == '#') continue;

        std::string fields[4];
        int fieldCount = splitLine(line, '|', fields, 4);
        int metres;
        if (fieldCount == 1 && isValidLocationName(fields[0])) {
            graph.addLocation(fields[0]);                        // a location on its own
        } else if (fieldCount == 3 && isValidLocationName(fields[0]) && isValidLocationName(fields[1]) &&
                   fields[0] != fields[1] && parseInt(fields[2], metres) && metres > 0 && metres <= 100000) {
            graph.addLocation(fields[0]);                        // harmless if it already exists
            graph.addLocation(fields[1]);
            if (graph.addConnection(fields[0], fields[1], metres)) {
                connections++;
            } else {
                std::cout << "  Location line " << lineNumber << " skipped (already connected).\n";
                skipped++;
            }
        } else {
            std::cout << "  Location line " << lineNumber << " skipped (bad format).\n";
            skipped++;
        }
    }
    in.close();
    std::cout << "Loaded " << graph.vertexCount() << " locations and " << connections
              << " connections from " << filename;
    if (skipped > 0) std::cout << " (" << skipped << " lines skipped)";
    std::cout << ".\n";
    return true;
}

bool CampusManager::saveLocations(const std::string& filename) const {
    std::ofstream out(filename.c_str());
    if (!out) {
        return false;
    }
    out << "# A|B|metres joins two locations with a walkway; a line with just a name is a location on its own.\n";
    graph.writeEdges(out);
    out.close();
    return true;
}

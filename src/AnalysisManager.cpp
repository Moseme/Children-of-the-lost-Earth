#include "AnalysisManager.h"
#include "Search.h"
#include "Sorting.h"
#include "HashTable.h"
#include "Tree.h"
#include "Request.h"
#include "Queue.h"
#include "Stack.h"
#include "PriorityQueue.h"
#include "Utils.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>

// =====================================================================
//  Helpers that build generated test data
// =====================================================================

// Fills arr with resources R000001, R000002, ... (already in sorted ID order).
static void fillSorted(Resource arr[], int n) {
    for (int i = 0; i < n; i++) {
        std::string digits = intToString(i + 1);
        while (digits.size() < 6) digits = "0" + digits;
        arr[i].id = "R" + digits;
        arr[i].name = "Resource " + digits;
        arr[i].capacity = i;
    }
}

// Shuffles the numbers 0..n-1 (Fisher-Yates). The seed is fixed so every run is repeatable.
static void shuffleOrder(int order[], int n) {
    for (int i = 0; i < n; i++) order[i] = i;
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = order[i];
        order[i] = order[j];
        order[j] = temp;
    }
}

static Resource* copyOf(const Resource src[], int n) {
    Resource* copy = new Resource[n];
    for (int i = 0; i < n; i++) copy[i] = src[i];
    return copy;
}

static double elapsedMs(clock_t start, clock_t finish) {
    return 1000.0 * (double)(finish - start) / (double)CLOCKS_PER_SEC;
}

static void printTitle(const std::string& title) {
    std::cout << "\n" << title << "\n" << std::string(title.size(), '=') << "\n";
}

// =====================================================================
//  1. Complexity summary
// =====================================================================

static void printRow(const char* operation, const char* structure, const char* time, const char* notes) {
    std::cout << std::left << std::setw(34) << operation << std::setw(24) << structure
              << std::setw(18) << time << notes << "\n";
}

void AnalysisManager::showComplexitySummary() const {
    printTitle("Complexity of the main operations (n = items, V = locations, E = walkways)");
    printRow("Operation", "Structure / algorithm", "Time", "Best / average / worst");
    std::cout << std::string(100, '-') << "\n";
    printRow("Insert at beginning or end", "Linked list (tail)", "O(1)", "same in all cases");
    printRow("Insert at position, delete by ID", "Linked list", "O(n)", "worst: walk to the spot");
    printRow("Linear search", "List or array", "O(n)", "best O(1), worst O(n)");
    printRow("Binary search", "Sorted array", "O(log n)", "best O(1), average and worst O(log n)");
    printRow("Insertion sort", "Array", "O(n^2)", "best O(n) if sorted, avg/worst O(n^2)");
    printRow("Merge sort", "Array + temp array", "O(n log n)", "same in all cases, extra O(n) space");
    printRow("Enqueue / dequeue", "Queue", "O(1)", "same in all cases");
    printRow("Push / pop / peek", "Stack", "O(1)", "same in all cases");
    printRow("Insert / remove highest", "Heap (priority queue)", "O(log n)", "worst case; peek is O(1)");
    printRow("Insert / find / remove by ID", "Hash table (chaining)", "O(1) average", "worst O(n) if all keys collide");
    printRow("Insert / search / remove", "BST", "O(log n) average", "worst O(n) if IDs arrive sorted");
    printRow("Inorder / preorder / postorder", "BST", "O(n)", "same in all cases");
    printRow("BFS, DFS, reachability", "Graph (adjacency list)", "O(V + E)", "same in all cases");
    printRow("Shortest path (Dijkstra)", "Graph (array scan)", "O(V^2)", "same in all cases");
    std::cout << "Run experiments 2 to 5 to see these growth rates measured.\n";
}

// =====================================================================
//  2. Searching experiment
// =====================================================================

void AnalysisManager::searchExperiment() const {
    printTitle("Experiment: average comparisons to find one ID (200 searches per size)");
    std::cout << "Generated data only; your real resources are not touched.\n";
    std::cout << std::left << std::setw(8) << "n" << std::setw(11) << "Linear" << std::setw(11) << "Binary"
              << std::setw(15) << "BST (random)" << std::setw(15) << "BST (sorted)" << "Hash\n";
    std::cout << std::string(66, '-') << "\n";

    const int sizes[5] = { 100, 500, 1000, 2000, 5000 };
    const int searches = 200;
    srand(2026);
    std::cout << std::fixed << std::setprecision(1);
    for (int s = 0; s < 5; s++) {
        int n = sizes[s];
        Resource* sorted = new Resource[n];
        fillSorted(sorted, n);
        int* order = new int[n];
        shuffleOrder(order, n);
        Resource* shuffled = new Resource[n];
        for (int i = 0; i < n; i++) shuffled[i] = sorted[order[i]];

        BST randomTree;                                    // IDs inserted in random order
        for (int i = 0; i < n; i++) randomTree.insert(&shuffled[i]);
        BST sortedTree;                                    // IDs inserted in sorted order (worst case)
        for (int i = 0; i < n; i++) sortedTree.insert(&sorted[i]);
        HashTable table;
        for (int i = 0; i < n; i++) table.insert(&sorted[i]);

        long linearTotal = 0, binaryTotal = 0, randomTotal = 0, sortedTotal = 0, hashTotal = 0;
        for (int k = 0; k < searches; k++) {
            const std::string id = sorted[rand() % n].id;  // always search for an ID that exists
            linearSearchById(shuffled, n, id, linearTotal);
            binarySearchById(sorted, n, id, binaryTotal);
            randomTree.search(id, randomTotal);
            sortedTree.search(id, sortedTotal);
            table.find(id, hashTotal);
        }
        std::cout << std::left << std::setw(8) << n
                  << std::setw(11) << (double)linearTotal / searches
                  << std::setw(11) << (double)binaryTotal / searches
                  << std::setw(15) << (double)randomTotal / searches
                  << std::setw(15) << (double)sortedTotal / searches
                  << (double)hashTotal / searches << "\n";
        delete[] sorted;
        delete[] order;
        delete[] shuffled;
    }
    std::cout.unsetf(std::ios::fixed);
    std::cout << "Linear grows like n/2, binary like log2(n), a balanced BST like 1.4 log2(n), and the hash\n"
              << "table stays near 1. A BST filled in sorted order degrades to linear search (its worst case).\n";
}

// =====================================================================
//  3. Hash table behaviour
// =====================================================================

void AnalysisManager::hashExperiment() const {
    printTitle("Experiment: hash table behaviour as it grows (starts with 53 buckets)");
    std::cout << std::left << std::setw(8) << "n" << std::setw(10) << "Buckets" << std::setw(13) << "Load factor"
              << std::setw(16) << "Longest chain" << "Avg comparisons\n";
    std::cout << std::string(64, '-') << "\n";
    const int sizes[5] = { 100, 500, 1000, 2000, 5000 };
    srand(2026);
    for (int s = 0; s < 5; s++) {
        int n = sizes[s];
        Resource* items = new Resource[n];
        fillSorted(items, n);
        HashTable table;
        for (int i = 0; i < n; i++) table.insert(&items[i]);
        long total = 0;
        for (int i = 0; i < n; i++) table.find(items[i].id, total);   // look every ID up once
        std::cout << std::left << std::setw(8) << n << std::setw(10) << table.bucketCount()
                  << std::fixed << std::setprecision(2) << std::setw(13) << table.loadFactor()
                  << std::setw(16) << table.longestChain() << (double)total / n << "\n";
        std::cout.unsetf(std::ios::fixed);
        delete[] items;
    }
    std::cout << "The table doubles when it fills, so the load factor stays at 1 or below and the average\n"
              << "lookup stays close to 1 comparison however large n becomes.\n";
}

// =====================================================================
//  4. Sorting experiment
// =====================================================================

static long countInsertion(const Resource src[], int n) {
    Resource* work = copyOf(src, n);
    long comparisons = 0;
    insertionSort(work, n, SORT_BY_ID, comparisons);
    delete[] work;
    return comparisons;
}

static long countMerge(const Resource src[], int n) {
    Resource* work = copyOf(src, n);
    long comparisons = 0;
    mergeSort(work, n, SORT_BY_ID, comparisons);
    delete[] work;
    return comparisons;
}

void AnalysisManager::sortExperiment() const {
    printTitle("Experiment: comparisons made when sorting by ID");
    std::cout << std::left << std::setw(7) << "n" << "| Insertion sort                  | Merge sort\n";
    std::cout << std::left << std::setw(7) << "" << "| " << std::setw(11) << "random" << std::setw(10) << "sorted"
              << std::setw(11) << "reversed" << "| " << std::setw(11) << "random" << std::setw(10) << "sorted" << "reversed\n";
    std::cout << std::string(74, '-') << "\n";
    const int sizes[4] = { 100, 500, 1000, 2000 };
    srand(2026);
    for (int s = 0; s < 4; s++) {
        int n = sizes[s];
        Resource* sortedInput = new Resource[n];
        fillSorted(sortedInput, n);
        Resource* reversedInput = new Resource[n];
        for (int i = 0; i < n; i++) reversedInput[i] = sortedInput[n - 1 - i];
        int* order = new int[n];
        shuffleOrder(order, n);
        Resource* randomInput = new Resource[n];
        for (int i = 0; i < n; i++) randomInput[i] = sortedInput[order[i]];

        std::cout << std::left << std::setw(7) << n << "| "
                  << std::setw(11) << countInsertion(randomInput, n)
                  << std::setw(10) << countInsertion(sortedInput, n)
                  << std::setw(11) << countInsertion(reversedInput, n) << "| "
                  << std::setw(11) << countMerge(randomInput, n)
                  << std::setw(10) << countMerge(sortedInput, n)
                  << countMerge(reversedInput, n) << "\n";
        delete[] sortedInput;
        delete[] reversedInput;
        delete[] order;
        delete[] randomInput;
    }
    std::cout << "Insertion sort: about n^2/4 comparisons on random data, n-1 on sorted data (best case) and\n"
              << "n^2/2 on reversed data (worst case). Merge sort stays near n log2(n) whatever the input.\n";
}

// =====================================================================
//  5. Heap, queue and stack speed
// =====================================================================

static double timeHeap(int n) {
    Request* reqs = new Request[n];
    for (int i = 0; i < n; i++) {
        reqs[i].id = makeRequestId(i + 1);
        reqs[i].priority = rand() % 4 + 1;
        reqs[i].time = "2026-10-19T09:00";
    }
    PriorityQueue heap;
    Request out;
    clock_t start = clock();
    for (int i = 0; i < n; i++) heap.insert(reqs[i]);       // n inserts, each O(log n)
    while (heap.removeHighest(out)) {}                       // n removals, each O(log n)
    clock_t finish = clock();
    delete[] reqs;
    return elapsedMs(start, finish);
}

static double timeQueue(int pairs) {
    RequestQueue queue;
    Request r;
    Request out;
    r.id = "REQ0001";
    clock_t start = clock();
    for (int i = 0; i < pairs; i++) {
        queue.enqueue(r);                                    // O(1)
        queue.dequeue(out);                                  // O(1)
    }
    clock_t finish = clock();
    return elapsedMs(start, finish);
}

static double timeStack(int pairs) {
    UndoStack stack;
    UndoAction action;
    UndoAction out;
    action.type = UNDO_ADD;
    clock_t start = clock();
    for (int i = 0; i < pairs; i++) {
        stack.push(action);                                  // O(1)
        stack.pop(out);                                      // O(1)
    }
    clock_t finish = clock();
    return elapsedMs(start, finish);
}

void AnalysisManager::speedExperiment() const {
    printTitle("Experiment: time in milliseconds (depends on your computer)");
    std::cout << "Priority queue (heap): n inserts followed by n remove-highest operations\n";
    std::cout << std::left << std::setw(10) << "n" << std::setw(14) << "Time (ms)" << "Time / (n log2 n), smaller is steadier\n";
    std::cout << std::string(60, '-') << "\n";
    const int heapSizes[4] = { 5000, 10000, 20000, 40000 };
    srand(2026);
    std::cout << std::fixed << std::setprecision(1);
    for (int i = 0; i < 4; i++) {
        int n = heapSizes[i];
        double ms = timeHeap(n);
        double log2n = 0;
        for (int v = n; v > 1; v /= 2) log2n++;
        std::cout << std::left << std::setw(10) << n << std::setw(14) << ms
                  << std::setprecision(6) << (ms / (n * log2n)) << std::setprecision(1) << "\n";
    }
    std::cout << "\nQueue and stack: n enqueue+dequeue pairs, and n push+pop pairs\n";
    std::cout << std::left << std::setw(10) << "n" << std::setw(18) << "Queue (ms)" << "Stack (ms)\n";
    std::cout << std::string(40, '-') << "\n";
    const int pairSizes[3] = { 100000, 200000, 400000 };
    for (int i = 0; i < 3; i++) {
        std::cout << std::left << std::setw(10) << pairSizes[i] << std::setw(18) << timeQueue(pairSizes[i])
                  << timeStack(pairSizes[i]) << "\n";
    }
    std::cout.unsetf(std::ios::fixed);
    std::cout << "Queue and stack time grows in step with n (each operation is O(1)). The heap grows a little\n"
              << "faster than n because every operation costs O(log n). Very small times can show as 0.0.\n";
}

// =====================================================================
//  6. Live snapshot
// =====================================================================

void AnalysisManager::showSnapshot() const {
    const HashTable& hash = resources.getHashIndex();
    const BST& tree = resources.getTreeIndex();
    printTitle("Live system snapshot");
    std::cout << "Resources in the list      : " << resources.count() << "\n"
              << "Entries in the hash table  : " << hash.size() << " (" << hash.bucketCount() << " buckets, load "
              << std::fixed << std::setprecision(2) << hash.loadFactor() << ", longest chain " << hash.longestChain() << ")\n";
    std::cout.unsetf(std::ios::fixed);
    std::cout << "Nodes in the BST           : " << tree.size() << " (height " << tree.height() << ")\n"
              << "Waiting normal requests    : " << requests.normalCount() << "\n"
              << "Waiting priority requests  : " << requests.priorityCount() << "\n"
              << "Processed requests         : " << requests.processedCount() << "\n"
              << "Actions that can be undone : " << resources.undoCount() << "\n"
              << "Campus locations           : " << campus.locationCount() << "\n";
    bool consistent = (resources.count() == hash.size() && resources.count() == tree.size());
    std::cout << "List, hash table and BST agree: " << (consistent ? "yes" : "NO - something is out of step") << "\n";
}

// =====================================================================
//  Menu
// =====================================================================

AnalysisManager::AnalysisManager(ResourceManager& rm, RequestManager& reqs, CampusManager& campusManager)
    : resources(rm), requests(reqs), campus(campusManager) {
}

void AnalysisManager::menu() {
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n--- SYSTEM ANALYSIS ---\n"
                  << "1. Complexity summary of the system\n"
                  << "2. Experiment: searching (linear, binary, BST, hash)\n"
                  << "3. Experiment: hash table behaviour\n"
                  << "4. Experiment: sorting (insertion sort vs merge sort)\n"
                  << "5. Experiment: heap, queue and stack speed\n"
                  << "6. Run all experiments (2 to 5)\n"
                  << "7. Live system snapshot\n"
                  << "0. Back to main menu\n";
        choice = readInt("Enter your choice: ", 0, 7);
        if (choice == 1) showComplexitySummary();
        else if (choice == 2) searchExperiment();
        else if (choice == 3) hashExperiment();
        else if (choice == 4) sortExperiment();
        else if (choice == 5) speedExperiment();
        else if (choice == 6) { searchExperiment(); hashExperiment(); sortExperiment(); speedExperiment(); }
        else if (choice == 7) showSnapshot();
    }
}

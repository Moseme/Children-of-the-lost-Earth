#include "ResourceManager.h"
#include "Utils.h"
#include "Sorting.h"
#include "Search.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

// =====================================================================
//  Small helpers
// =====================================================================

bool ResourceManager::isValidText(const std::string& text) const {
    if (text.empty()) return false;
    for (int i = 0; i < (int)text.size(); i++) {
        if (text[i] == '|') return false;      // '|' separates fields in the data file
    }
    return true;
}

// Ask for all fields of a resource. Returns false if the user typed something invalid.
bool ResourceManager::readResourceFields(Resource& r, bool askForId) {
    if (askForId) {
        r.id = readLine("Resource ID (e.g. LAB105): ");
        if (!isValidText(r.id)) { std::cout << "ID cannot be empty or contain '|'.\n"; return false; }
        if (list.findById(r.id) != nullptr) {
            std::cout << "A resource with ID " << r.id << " already exists.\n";
            return false;
        }
    }
    r.name = readLine("Name: ");
    if (!isValidText(r.name)) { std::cout << "Name cannot be empty or contain '|'.\n"; return false; }
    r.type = readLine("Type (e.g. Computer Laboratory): ");
    if (!isValidText(r.type)) { std::cout << "Type cannot be empty or contain '|'.\n"; return false; }
    r.location = readLine("Location (e.g. Building 32): ");
    if (!isValidText(r.location)) { std::cout << "Location cannot be empty or contain '|'.\n"; return false; }
    r.capacity = readInt("Capacity (0 - 100000): ", 0, 100000);
    r.available = (readInt("Status (1 = Available, 0 = Unavailable): ", 0, 1) == 1);
    return true;
}

// =====================================================================
//  Module 1: Resource Management  (add / remove / update / display)
// =====================================================================

void ResourceManager::addResource(int where) {
    Resource r;
    if (!readResourceFields(r, true)) {
        std::cout << "Resource was NOT added.\n";
        return;
    }
    if (where == 1) {
        list.insertAtBeginning(r);
    } else if (where == 2) {
        list.insertAtEnd(r);
    } else {
        int position = readInt("Position (1 to " + std::to_string(list.size() + 1) + "): ", 1, list.size() + 1);
        list.insertAtPosition(position, r);
    }
    std::cout << "Resource " << r.id << " added.\n";
    // TODO (team): also insert r into the BST and the HashTable.
}

void ResourceManager::removeResource() {
    if (list.isEmpty()) {
        std::cout << "There are no resources to remove.\n";
        return;
    }
    std::string id = readLine("ID of the resource to remove: ");
    Resource removed;
    if (list.removeById(id, removed)) {
        std::cout << "Removed:\n";
        printResourceHeader();
        printResourceRow(removed);
        // TODO (Student B): push 'removed' onto the undo stack here.
        // TODO (team): also remove it from the BST and the HashTable.
    } else {
        std::cout << "No resource with ID '" << id << "' was found.\n";
    }
}

void ResourceManager::updateResource() {
    std::string id = readLine("ID of the resource to update: ");
    Resource* r = list.findById(id);
    if (r == nullptr) {
        std::cout << "No resource with ID '" << id << "' was found.\n";
        return;
    }
    std::cout << "Current values:\n";
    printResourceHeader();
    printResourceRow(*r);
    std::cout << "Press Enter to keep the current value.\n";

    // TODO (Student B): save a copy of *r on the undo stack before changing it.
    std::string text;

    text = readLine("New name: ");
    if (!text.empty()) {
        if (isValidText(text)) r->name = text; else std::cout << "Invalid name, kept the old one.\n";
    }
    text = readLine("New type: ");
    if (!text.empty()) {
        if (isValidText(text)) r->type = text; else std::cout << "Invalid type, kept the old one.\n";
    }
    text = readLine("New location: ");
    if (!text.empty()) {
        if (isValidText(text)) r->location = text; else std::cout << "Invalid location, kept the old one.\n";
    }
    text = readLine("New capacity: ");
    if (!text.empty()) {
        int value;
        if (parseInt(text, value) && value >= 0 && value <= 100000) r->capacity = value;
        else std::cout << "Invalid capacity, kept the old one.\n";
    }
    text = readLine("New status (1 = Available, 0 = Unavailable): ");
    if (!text.empty()) {
        int value;
        if (parseInt(text, value) && (value == 0 || value == 1)) r->available = (value == 1);
        else std::cout << "Invalid status, kept the old one.\n";
    }
    std::cout << "Updated:\n";
    printResourceHeader();
    printResourceRow(*r);
}

// Creates many fake resources so that sorting/searching can be tested on a large data set.
void ResourceManager::generateTestData() {
    int n = readInt("How many test resources to add (1 - 5000)? ", 1, 5000);
    const char* types[4] = { "Computer Laboratory", "Lecture Room", "Repair Workshop", "Equipment" };
    srand(12345);                     // fixed seed: same data every time, so tests can be repeated
    int added = 0;
    for (int i = 1; i <= n; i++) {
        std::string number = std::to_string(i);
        while (number.size() < 5) number = "0" + number;       // 1 -> 00001
        Resource r;
        r.id = "GEN" + number;
        if (list.findById(r.id) != nullptr) continue;          // skip if it already exists
        r.name = "Test Resource " + std::to_string(rand() % 100000);
        r.type = types[rand() % 4];
        r.location = "Building " + std::to_string(rand() % 40 + 1);
        r.capacity = rand() % 200 + 1;
        r.available = (rand() % 2 == 0);
        list.insertAtEnd(r);
        added++;
    }
    std::cout << added << " test resources added. Total resources: " << list.size() << "\n";
}

void ResourceManager::resourceMenu() {
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n--- RESOURCE MANAGEMENT (" << list.size() << " resources) ---\n"
                  << "1. Add resource at the beginning of the list\n"
                  << "2. Add resource at the end of the list\n"
                  << "3. Add resource at a chosen position\n"
                  << "4. Remove a resource\n"
                  << "5. Update a resource\n"
                  << "6. Display all resources\n"
                  << "7. Generate test data (for large data tests)\n"
                  << "0. Back to main menu\n";
        choice = readInt("Enter your choice: ", 0, 7);
        if (choice == 1) addResource(1);
        else if (choice == 2) addResource(2);
        else if (choice == 3) addResource(3);
        else if (choice == 4) removeResource();
        else if (choice == 5) updateResource();
        else if (choice == 6) list.display();
        else if (choice == 7) generateTestData();
    }
}

// =====================================================================
//  Module 6 (part): Sorting
// =====================================================================

static void displayArray(const Resource arr[], int n, int limit) {
    printResourceHeader();
    int shown = (n < limit) ? n : limit;
    for (int i = 0; i < shown; i++) {
        printResourceRow(arr[i]);
    }
    if (n > shown) {
        std::cout << "... (showing the first " << shown << " of " << n << ")\n";
    }
}

// Sorts a COPY of the list, so the original order in the list is not changed.
void ResourceManager::showSorted(int algorithm, int criterion) {
    int n;
    Resource* arr = list.toArray(n);
    if (arr == nullptr) {
        std::cout << "There are no resources to sort.\n";
        return;
    }
    long comparisons = 0;
    if (algorithm == 1) {
        insertionSort(arr, n, criterion, comparisons);
        std::cout << "\nSorted by " << criterionName(criterion) << " using INSERTION SORT:\n";
    } else {
        mergeSort(arr, n, criterion, comparisons);
        std::cout << "\nSorted by " << criterionName(criterion) << " using MERGE SORT:\n";
    }
    displayArray(arr, n, 20);
    std::cout << "Comparisons made: " << comparisons << "\n";
    delete[] arr;
}

// Runs both algorithms on identical copies of the data and prints the results side by side.
void ResourceManager::compareSortingAlgorithms(int criterion) {
    int n;
    Resource* a = list.toArray(n);
    if (a == nullptr) {
        std::cout << "There are no resources to sort.\n";
        return;
    }
    int n2;
    Resource* b = list.toArray(n2);

    long insertionComparisons = 0;
    long mergeComparisons = 0;

    clock_t start = clock();
    insertionSort(a, n, criterion, insertionComparisons);
    clock_t middle = clock();
    mergeSort(b, n, criterion, mergeComparisons);
    clock_t finish = clock();

    double insertionMs = 1000.0 * (middle - start) / CLOCKS_PER_SEC;
    double mergeMs = 1000.0 * (finish - middle) / CLOCKS_PER_SEC;

    std::cout << "\nSorting " << n << " resources by " << criterionName(criterion) << ":\n";
    std::cout << "  Insertion sort : " << insertionComparisons << " comparisons, "
              << insertionMs << " ms   (O(n^2) average)\n";
    std::cout << "  Merge sort     : " << mergeComparisons << " comparisons, "
              << mergeMs << " ms   (O(n log n) always)\n";
    std::cout << "Tip: use option 7 in Resource Management to add thousands of test resources,\n"
              << "then compare again to see the difference grow.\n";
    delete[] a;
    delete[] b;
}

void ResourceManager::sortMenu() {
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n--- SORT RESOURCES ---\n"
                  << "1. Insertion sort\n"
                  << "2. Merge sort\n"
                  << "3. Compare both algorithms\n"
                  << "0. Back to main menu\n";
        choice = readInt("Enter your choice: ", 0, 3);
        if (choice == 0) break;

        std::cout << "Sort by:  1. Resource ID   2. Name   3. Capacity   4. Type\n";
        int criterion = readInt("Enter your choice (0 = cancel): ", 0, 4);
        if (criterion == 0) continue;

        if (choice == 3) compareSortingAlgorithms(criterion);
        else showSorted(choice, criterion);
    }
}

// =====================================================================
//  Module 6 (part): Searching
// =====================================================================

// Linear search: look at the resources one after another in their stored order.
void ResourceManager::searchByIdLinear() {
    int n;
    Resource* arr = list.toArray(n);
    if (arr == nullptr) { std::cout << "There are no resources to search.\n"; return; }
    std::string id = readLine("Resource ID to search for: ");
    long comparisons = 0;
    int index = linearSearchById(arr, n, id, comparisons);
    if (index >= 0) {
        std::cout << "Found (linear search):\n";
        printResourceHeader();
        printResourceRow(arr[index]);
    } else {
        std::cout << "Resource '" << id << "' was not found.\n";
    }
    std::cout << "Comparisons made: " << comparisons << " (out of " << n << " resources)\n";
    delete[] arr;
}

// Binary search needs a SORTED array, so we sort a copy by ID first (merge sort).
void ResourceManager::searchByIdBinary() {
    int n;
    Resource* arr = list.toArray(n);
    if (arr == nullptr) { std::cout << "There are no resources to search.\n"; return; }
    long sortComparisons = 0;
    mergeSort(arr, n, SORT_BY_ID, sortComparisons);     // required before binary search!
    std::string id = readLine("Resource ID to search for: ");
    long comparisons = 0;
    int index = binarySearchById(arr, n, id, comparisons);
    if (index >= 0) {
        std::cout << "Found (binary search):\n";
        printResourceHeader();
        printResourceRow(arr[index]);
    } else {
        std::cout << "Resource '" << id << "' was not found.\n";
    }
    std::cout << "Comparisons made by the search: " << comparisons << " (out of " << n << " resources)\n";
    std::cout << "(Sorting the copy first cost " << sortComparisons << " comparisons.)\n";
    delete[] arr;
}

void ResourceManager::searchByNameLinear() {
    if (list.isEmpty()) { std::cout << "There are no resources to search.\n"; return; }
    std::string name = readLine("Exact resource name to search for: ");
    Resource* r = list.findByName(name);
    if (r != nullptr) {
        std::cout << "Found:\n";
        printResourceHeader();
        printResourceRow(*r);
    } else {
        std::cout << "No resource named '" << name << "' was found.\n";
    }
}

void ResourceManager::searchMenu() {
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n--- SEARCH RESOURCES ---\n"
                  << "1. Linear search by ID\n"
                  << "2. Binary search by ID\n"
                  << "3. Linear search by name\n"
                  << "0. Back to main menu\n";
        choice = readInt("Enter your choice: ", 0, 3);
        if (choice == 1) searchByIdLinear();
        else if (choice == 2) searchByIdBinary();
        else if (choice == 3) searchByNameLinear();
    }
}

// =====================================================================
//  Module 10: Data persistence (text file)
// =====================================================================

bool ResourceManager::loadFromFile(const std::string& filename) {
    std::ifstream in(filename.c_str());
    if (!in) {
        return false;                       // file does not exist (yet)
    }
    std::string line;
    int lineNumber = 0;
    int loaded = 0;
    int skipped = 0;
    while (std::getline(in, line)) {
        lineNumber++;
        line = trim(line);
        if (line.empty() || line[0] == '#') continue;      // blank line or comment

        std::string fields[7];
        int fieldCount = splitLine(line, '|', fields, 7);
        int capacity;
        int available;
        if (fieldCount != 6 ||
            !isValidText(fields[0]) || !isValidText(fields[1]) ||
            !isValidText(fields[2]) || !isValidText(fields[3]) ||
            !parseInt(fields[4], capacity) || capacity < 0 ||
            !parseInt(fields[5], available) || (available != 0 && available != 1)) {
            std::cout << "  Line " << lineNumber << " skipped (bad format).\n";
            skipped++;
            continue;
        }
        if (list.findById(fields[0]) != nullptr) {
            std::cout << "  Line " << lineNumber << " skipped (duplicate ID " << fields[0] << ").\n";
            skipped++;
            continue;
        }
        Resource r(fields[0], fields[1], fields[2], fields[3], capacity, available == 1);
        list.insertAtEnd(r);
        loaded++;
    }
    in.close();
    std::cout << "Loaded " << loaded << " resources from " << filename;
    if (skipped > 0) std::cout << " (" << skipped << " lines skipped)";
    std::cout << ".\n";
    return true;
}

bool ResourceManager::saveToFile(const std::string& filename) const {
    std::ofstream out(filename.c_str());
    if (!out) {
        return false;
    }
    out << "# id|name|type|location|capacity|available(1/0)\n";
    int n;
    Resource* arr = list.toArray(n);
    for (int i = 0; i < n; i++) {
        out << arr[i].id << "|" << arr[i].name << "|" << arr[i].type << "|"
            << arr[i].location << "|" << arr[i].capacity << "|"
            << (arr[i].available ? 1 : 0) << "\n";
    }
    delete[] arr;      // delete[] on nullptr is safe
    out.close();
    return true;
}

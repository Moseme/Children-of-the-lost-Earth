// SHARED: integration point. Each owner wires their menu section here.
#include <iostream>
#include "ResourceManager.h"
#include "Utils.h"

static const char* RESOURCE_FILE = "data/resources.txt";

static void printMenu() {
    std::cout << "\n================================================\n"
              << "  CS3400 CAMPUS TECH SUPPORT FACILITIES SYSTEM\n"
              << "================================================\n"
              << "1. Resource Management\n"
              << "2. Search Resources\n"
              << "3. Sort Resources\n"
              << "4. Resource Requests\n"
              << "5. Priority Requests\n"
              << "6. Undo Operations\n"
              << "7. Tree Operations\n"
              << "8. Hash Table Lookup\n"
              << "9. Campus Navigation\n"
              << "10. System Analysis\n"
              << "0. Exit\n";
}

int main() {
    ResourceManager manager;

    // Load data when the program starts.
    if (!manager.loadFromFile(RESOURCE_FILE)) {
        std::cout << "Note: " << RESOURCE_FILE << " not found. Starting with no resources.\n";
    }
    // TODO (team): after loading, fill Student C's BST and HashTable from manager.getAllAsArray(n).

    int choice = -1;
    while (choice != 0) {
        printMenu();
        choice = readInt("Enter your choice: ", 0, 10);   // rejects letters and out-of-range numbers
        switch (choice) {
            case 0: break;
            case 1: manager.resourceMenu(); break;
            case 2: manager.searchMenu(); break;
            case 3: manager.sortMenu(); break;
            case 4: case 5: case 6:   std::cout << "[Student B: not implemented yet]\n"; break;
            case 7: case 8: case 9:   std::cout << "[Student C: not implemented yet]\n"; break;
            case 10: std::cout << "[Shared: not implemented yet]\n"; break;
        }
    }

    // Save changes when the program terminates.
    if (manager.saveToFile(RESOURCE_FILE)) {
        std::cout << "Resources saved to " << RESOURCE_FILE << ".\n";
    } else {
        std::cout << "Warning: could not save to " << RESOURCE_FILE << ".\n";
    }
    std::cout << "Goodbye.\n";
    return 0;
}

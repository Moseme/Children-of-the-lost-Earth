// SHARED: integration point. Each owner wires their menu section here.
#include <iostream>
#include "ResourceManager.h"
#include "RequestManager.h"
#include "CampusManager.h"
#include "Utils.h"

static const char* RESOURCE_FILE = "data/resources.txt";
static const char* REQUEST_FILE = "data/requests.txt";
static const char* LOCATION_FILE = "data/locations.txt";

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
    RequestManager requests(manager);     // the request system needs access to the resources
    CampusManager campus(manager);        // tree, hash table and graph menus

    // Load data when the program starts.
    if (!manager.loadFromFile(RESOURCE_FILE)) {
        std::cout << "Note: " << RESOURCE_FILE << " not found. Starting with no resources.\n";
    }
    if (!requests.loadFromFile(REQUEST_FILE)) {
        std::cout << "Note: " << REQUEST_FILE << " not found. Starting with no requests.\n";
    }
    if (!campus.loadLocations(LOCATION_FILE)) {
        std::cout << "Note: " << LOCATION_FILE << " not found. Starting with no locations.\n";
    }
    // (The hash table and BST fill themselves while the resources load.)

    int choice = -1;
    while (choice != 0) {
        printMenu();
        choice = readInt("Enter your choice: ", 0, 10);   // rejects letters and out-of-range numbers
        switch (choice) {
            case 0: break;
            case 1: manager.resourceMenu(); break;
            case 2: manager.searchMenu(); break;
            case 3: manager.sortMenu(); break;
            case 4: requests.requestMenu(); break;
            case 5: requests.priorityMenu(); break;
            case 6: manager.undoMenu(); break;
            case 7: campus.treeMenu(); break;
            case 8: campus.hashMenu(); break;
            case 9: campus.navigationMenu(); break;
            case 10: std::cout << "[Shared: not implemented yet]\n"; break;
        }
    }

    // Save changes when the program terminates.
    if (manager.saveToFile(RESOURCE_FILE)) {
        std::cout << "Resources saved to " << RESOURCE_FILE << ".\n";
    } else {
        std::cout << "Warning: could not save to " << RESOURCE_FILE << ".\n";
    }
    if (requests.saveToFile(REQUEST_FILE)) {
        std::cout << "Requests saved to " << REQUEST_FILE << ".\n";
    } else {
        std::cout << "Warning: could not save to " << REQUEST_FILE << ".\n";
    }
    if (campus.saveLocations(LOCATION_FILE)) {
        std::cout << "Locations saved to " << LOCATION_FILE << ".\n";
    } else {
        std::cout << "Warning: could not save to " << LOCATION_FILE << ".\n";
    }
    std::cout << "Goodbye.\n";
    return 0;
}

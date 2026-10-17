// SHARED: integration point. Each owner wires their menu section here.
#include <iostream>
#include <limits>

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
              << "0. Exit\n"
              << "Enter your choice: ";
}

int main() {
    int choice = -1;
    while (choice != 0) {
        printMenu();
        if (!(std::cin >> choice)) {              // invalid input handling
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Enter a number.\n";
            choice = -1;
            continue;
        }
        switch (choice) {
            case 0: std::cout << "Goodbye.\n"; break;
            case 1: case 2: case 3:   std::cout << "[Student A: not implemented yet]\n"; break;
            case 4: case 5: case 6:   std::cout << "[Student B: not implemented yet]\n"; break;
            case 7: case 8: case 9:   std::cout << "[Student C: not implemented yet]\n"; break;
            case 10: std::cout << "[Shared: not implemented yet]\n"; break;
            default: std::cout << "Invalid option.\n";
        }
    }
    return 0;
}

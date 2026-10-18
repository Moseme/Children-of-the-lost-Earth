#include "RequestManager.h"
#include "RequestSort.h"
#include "Utils.h"
#include <iostream>
#include <fstream>

// A text field must not be empty and must not contain '|' (the file separator).
static bool isValidField(const std::string& text) {
    if (text.empty()) return false;
    for (int i = 0; i < (int)text.size(); i++) {
        if (text[i] == '|') return false;
    }
    return true;
}

RequestManager::RequestManager(ResourceManager& rm) : resources(rm), nextNumber(1) {
}

bool RequestManager::idInUse(const std::string& id) const {
    return normalQueue.containsId(id) || priorityHeap.containsId(id) || processedLog.containsId(id);
}

// =====================================================================
//  Submitting requests
// =====================================================================

// Asks who is requesting and which resource. Returns false if the answers are not valid.
bool RequestManager::readRequestBasics(Request& r) {
    r.user = readLine("Your user name / ID: ");
    if (!isValidField(r.user)) {
        std::cout << "User cannot be empty or contain '|'. Request NOT submitted.\n";
        return false;
    }
    r.resourceId = readLine("ID of the resource you need (e.g. LAB101): ");
    Resource* wanted = resources.findResource(r.resourceId);
    if (wanted == NULL) {
        std::cout << "No resource with ID '" << r.resourceId << "' exists. Request NOT submitted.\n";
        return false;
    }
    std::cout << "Requesting: " << wanted->name << " (" << wanted->type << ", " << wanted->location << ")\n";
    r.id = makeRequestId(nextNumber);
    r.time = currentTimestamp();
    r.status = "Pending";
    return true;
}

void RequestManager::submitNormalRequest() {
    Request r;
    if (!readRequestBasics(r)) return;
    r.priority = PRIORITY_NORMAL;
    r.isPriorityRequest = false;
    nextNumber++;
    normalQueue.enqueue(r);                         // joins the END of the line
    std::cout << "Request " << r.id << " submitted. Position in queue: " << normalQueue.size() << "\n";
}

void RequestManager::submitPriorityRequest() {
    Request r;
    if (!readRequestBasics(r)) return;
    std::cout << "Priority:  1. Emergency   2. High   3. Normal   4. Low\n";
    r.priority = readInt("Enter priority (1-4): ", 1, 4);
    r.isPriorityRequest = true;
    nextNumber++;
    priorityHeap.insert(r);                         // the heap puts it in the right place
    std::cout << "Request " << r.id << " submitted with priority " << r.priority
              << " (" << priorityName(r.priority) << ").\n";
}

// =====================================================================
//  Processing requests
// =====================================================================

// Decision rule (lab allocation): approve if the resource exists and is available,
// and mark it Unavailable (allocated). Otherwise reject.
void RequestManager::decide(Request& r) {
    Resource* wanted = resources.findResource(r.resourceId);
    std::string reason;
    if (wanted == NULL) {
        r.status = "Rejected";
        reason = "the resource no longer exists";
    } else if (!wanted->available) {
        r.status = "Rejected";
        reason = "the resource is not available";
    } else {
        wanted->available = false;                 // allocate it to this request
        r.status = "Approved";
        reason = "resource allocated (it is now Unavailable)";
    }
    processedLog.enqueue(r);                        // keep a record of the finished request
    std::cout << "Processed:\n";
    printRequestHeader();
    printRequestRow(r);
    std::cout << "Result: " << r.status << " - " << reason << ".\n";
}

void RequestManager::processNextNormal() {
    Request r;
    if (!normalQueue.dequeue(r)) {                  // dequeue takes the OLDEST request
        std::cout << "The normal request queue is empty. Nothing to process.\n";
        return;
    }
    decide(r);
}

void RequestManager::processNextPriority() {
    Request r;
    if (!priorityHeap.removeHighest(r)) {           // takes the MOST URGENT request
        std::cout << "There are no priority requests waiting. Nothing to process.\n";
        return;
    }
    decide(r);
}

void RequestManager::viewProcessed() const {
    if (processedLog.isEmpty()) {
        std::cout << "No requests have been processed yet.\n";
        return;
    }
    processedLog.display();
}

void RequestManager::releaseResource() {
    std::string id = readLine("ID of the resource to release (mark Available): ");
    Resource* r = resources.findResource(id);
    if (r == NULL) {
        std::cout << "No resource with ID '" << id << "' was found.\n";
    } else if (r->available) {
        std::cout << r->id << " is already Available.\n";
    } else {
        r->available = true;
        std::cout << r->id << " (" << r->name << ") is now Available again.\n";
    }
}

// =====================================================================
//  Sorting all requests (pending + processed)
// =====================================================================

void RequestManager::sortAllRequests() const {
    int n1, n2, n3;
    Request* a = normalQueue.toArray(n1);
    Request* b = priorityHeap.toArray(n2);
    Request* c = processedLog.toArray(n3);
    int total = n1 + n2 + n3;
    if (total == 0) {
        std::cout << "There are no requests to sort.\n";
        return;
    }
    std::cout << "Sort by:  1. Submission time   2. Priority   3. Status\n";
    int criterion = readInt("Enter your choice (0 = cancel): ", 0, 3);
    if (criterion != 0) {
        Request* all = new Request[total];
        int k = 0;
        for (int i = 0; i < n1; i++) all[k++] = a[i];
        for (int i = 0; i < n2; i++) all[k++] = b[i];
        for (int i = 0; i < n3; i++) all[k++] = c[i];

        long comparisons = 0;
        mergeSortRequests(all, total, criterion, comparisons);
        std::cout << "\nAll " << total << " requests sorted by " << requestCriterionName(criterion) << ":\n";
        printRequestHeader();
        for (int i = 0; i < total; i++) printRequestRow(all[i]);
        std::cout << "Comparisons made (merge sort): " << comparisons << "\n";
        delete[] all;
    }
    delete[] a;      // delete[] on NULL is safe
    delete[] b;
    delete[] c;
}

// =====================================================================
//  Menus
// =====================================================================

void RequestManager::requestMenu() {
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n--- RESOURCE REQUESTS: normal queue, first come first served ("
                  << normalQueue.size() << " waiting) ---\n"
                  << "1. Submit a request (enqueue)\n"
                  << "2. View pending requests (front of the queue first)\n"
                  << "3. View the next request to be served (peek front)\n"
                  << "4. Process the next request (dequeue)\n"
                  << "5. View processed requests\n"
                  << "6. Release a resource (mark it Available again)\n"
                  << "7. Sort all requests\n"
                  << "0. Back to main menu\n";
        choice = readInt("Enter your choice: ", 0, 7);
        if (choice == 1) {
            submitNormalRequest();
        } else if (choice == 2) {
            normalQueue.display();
        } else if (choice == 3) {
            const Request* front = normalQueue.peekFront();
            if (front == NULL) {
                std::cout << "The queue is empty.\n";
            } else {
                std::cout << "Next to be served:\n";
                printRequestHeader();
                printRequestRow(*front);
            }
        } else if (choice == 4) {
            processNextNormal();
        } else if (choice == 5) {
            viewProcessed();
        } else if (choice == 6) {
            releaseResource();
        } else if (choice == 7) {
            sortAllRequests();
        }
    }
}

void RequestManager::priorityMenu() {
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n--- PRIORITY REQUESTS: most urgent first ("
                  << priorityHeap.size() << " waiting) ---\n"
                  << "1. Submit a priority request (heap insert)\n"
                  << "2. View the most urgent request (peek)\n"
                  << "3. View all waiting requests in processing order\n"
                  << "4. Process the most urgent request (remove highest)\n"
                  << "5. Show how the heap is stored in its array\n"
                  << "0. Back to main menu\n";
        choice = readInt("Enter your choice: ", 0, 5);
        if (choice == 1) {
            submitPriorityRequest();
        } else if (choice == 2) {
            const Request* top = priorityHeap.peekHighest();
            if (top == NULL) {
                std::cout << "No priority requests are waiting.\n";
            } else {
                std::cout << "Most urgent request:\n";
                printRequestHeader();
                printRequestRow(*top);
            }
        } else if (choice == 3) {
            priorityHeap.displayInPriorityOrder();
        } else if (choice == 4) {
            processNextPriority();
        } else if (choice == 5) {
            priorityHeap.displayHeapArray();
        }
    }
}

// =====================================================================
//  Data persistence
// =====================================================================

bool RequestManager::loadFromFile(const std::string& filename) {
    std::ifstream in(filename.c_str());
    if (!in) {
        return false;
    }
    std::string line;
    int lineNumber = 0;
    int loaded = 0;
    int skipped = 0;
    while (std::getline(in, line)) {
        lineNumber++;
        line = trim(line);
        if (line.empty() || line[0] == '#') continue;

        std::string fields[8];
        int fieldCount = splitLine(line, '|', fields, 8);
        int priority;
        bool valid = (fieldCount == 6 || fieldCount == 7);
        if (valid) {
            valid = isValidField(fields[0]) && isValidField(fields[1]) &&
                    isValidField(fields[2]) && isValidField(fields[3]) &&
                    parseInt(fields[4], priority) && priority >= 1 && priority <= 4 &&
                    (fields[5] == "Pending" || fields[5] == "Approved" || fields[5] == "Rejected");
        }
        char kind = 'N';
        if (valid && fieldCount == 7) {
            if (fields[6] == "N" || fields[6] == "P") kind = fields[6][0];
            else valid = false;
        }
        if (!valid) {
            std::cout << "  Request line " << lineNumber << " skipped (bad format).\n";
            skipped++;
            continue;
        }
        if (idInUse(fields[0])) {
            std::cout << "  Request line " << lineNumber << " skipped (duplicate ID " << fields[0] << ").\n";
            skipped++;
            continue;
        }

        Request r;
        r.id = fields[0];
        r.user = fields[1];
        r.resourceId = fields[2];
        r.time = fields[3];
        r.priority = priority;
        r.status = fields[5];
        r.isPriorityRequest = (kind == 'P');

        if (r.status != "Pending") {
            processedLog.enqueue(r);
        } else if (r.isPriorityRequest) {
            priorityHeap.insert(r);
        } else {
            normalQueue.enqueue(r);
        }
        loaded++;

        // Keep new IDs unique: continue numbering after the largest REQ number seen.
        int number;
        if (r.id.size() > 3 && r.id.substr(0, 3) == "REQ" && parseInt(r.id.substr(3), number)) {
            if (number >= nextNumber) nextNumber = number + 1;
        }
    }
    in.close();
    std::cout << "Loaded " << loaded << " requests from " << filename;
    if (skipped > 0) std::cout << " (" << skipped << " lines skipped)";
    std::cout << ".\n";
    return true;
}

bool RequestManager::saveToFile(const std::string& filename) const {
    std::ofstream out(filename.c_str());
    if (!out) {
        return false;
    }
    out << "# id|user|resourceId|time|priority|status|kind(N=normal queue, P=priority heap)\n";

    int n;
    Request* arr = normalQueue.toArray(n);
    for (int i = 0; i < n; i++) {
        out << arr[i].id << "|" << arr[i].user << "|" << arr[i].resourceId << "|" << arr[i].time << "|"
            << arr[i].priority << "|" << arr[i].status << "|N\n";
    }
    delete[] arr;

    arr = priorityHeap.toArray(n);
    for (int i = 0; i < n; i++) {
        out << arr[i].id << "|" << arr[i].user << "|" << arr[i].resourceId << "|" << arr[i].time << "|"
            << arr[i].priority << "|" << arr[i].status << "|P\n";
    }
    delete[] arr;

    arr = processedLog.toArray(n);
    for (int i = 0; i < n; i++) {
        out << arr[i].id << "|" << arr[i].user << "|" << arr[i].resourceId << "|" << arr[i].time << "|"
            << arr[i].priority << "|" << arr[i].status << "|" << (arr[i].isPriorityRequest ? "P" : "N") << "\n";
    }
    delete[] arr;
    out.close();
    return true;
}

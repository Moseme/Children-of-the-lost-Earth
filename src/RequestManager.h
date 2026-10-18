// OWNER: Student B.
// Runs the request system: the normal FIFO queue (main menu option 4),
// the priority heap (option 5), request processing, and saving/loading requests.
//
// Two separate waiting lines exist on purpose (as the brief asks):
//   normalQueue  : ordinary requests, served strictly first-come-first-served (FIFO)
//   priorityHeap : requests with a priority 1-4, the most urgent is served first
// Finished requests (Approved / Rejected) are kept in processedLog.
//
// File format, one request per line:
//   id|user|resourceId|time|priority|status|kind     kind: N = normal queue, P = priority heap
// Lines that are empty or start with '#' are ignored.
#ifndef REQUESTMANAGER_H
#define REQUESTMANAGER_H
#include <string>
#include "Request.h"
#include "Queue.h"
#include "PriorityQueue.h"
#include "ResourceManager.h"

class RequestManager {
private:
    ResourceManager& resources;   // needed to check, allocate and release resources
    RequestQueue normalQueue;
    PriorityQueue priorityHeap;
    RequestQueue processedLog;
    int nextNumber;               // number used for the next request ID

    RequestManager(const RequestManager&);               // copying forbidden
    RequestManager& operator=(const RequestManager&);

    bool idInUse(const std::string& id) const;
    bool readRequestBasics(Request& r);      // asks for user + resource, checks them
    void submitNormalRequest();
    void submitPriorityRequest();
    void decide(Request& r);                 // approve or reject, then record in processedLog
    void processNextNormal();
    void processNextPriority();
    void viewProcessed() const;
    void releaseResource();
    void sortAllRequests() const;

public:
    explicit RequestManager(ResourceManager& rm);

    // Counts, used by the System Analysis snapshot.
    int normalCount() const { return normalQueue.size(); }
    int priorityCount() const { return priorityHeap.size(); }
    int processedCount() const { return processedLog.size(); }

    void requestMenu();       // main menu option 4
    void priorityMenu();      // main menu option 5

    bool loadFromFile(const std::string& filename);
    bool saveToFile(const std::string& filename) const;
};

#endif

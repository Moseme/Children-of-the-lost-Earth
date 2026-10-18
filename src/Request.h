// OWNER: Student B.
// A Request is one user's request for one campus resource (for example "Lab 101").
// Everything about a request is kept in this simple struct.
#ifndef REQUEST_H
#define REQUEST_H
#include <string>

// Priority levels: a SMALLER number means MORE urgent.
enum RequestPriority {
    PRIORITY_EMERGENCY = 1,
    PRIORITY_HIGH = 2,
    PRIORITY_NORMAL = 3,
    PRIORITY_LOW = 4
};

struct Request {
    std::string id;          // e.g. REQ0007 (made by the system, always 4 digits)
    std::string user;        // who asked, e.g. student01
    std::string resourceId;  // which resource is wanted, e.g. LAB101
    std::string time;        // when it was submitted, e.g. 2026-10-19T09:00
    int priority;            // 1 = Emergency ... 4 = Low
    std::string status;      // "Pending", "Approved" or "Rejected"
    bool isPriorityRequest;  // true = submitted through the priority (heap) system

    Request() : priority(PRIORITY_NORMAL), status("Pending"), isPriorityRequest(false) {}
};

const char* priorityName(int priority);          // 1 -> "Emergency", 2 -> "High", ...
std::string currentTimestamp();                  // now, as YYYY-MM-DDTHH:MM
std::string makeRequestId(int number);           // 7 -> "REQ0007"

// The rule the priority queue uses to decide who goes first:
//   1) lower priority number first, 2) if equal, earlier time first, 3) if equal, smaller ID first.
bool requestComesBefore(const Request& a, const Request& b);

void printRequestHeader();
void printRequestRow(const Request& r);

#endif

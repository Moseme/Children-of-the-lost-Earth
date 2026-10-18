#include "Request.h"
#include "Utils.h"
#include <iostream>
#include <iomanip>
#include <ctime>

const char* priorityName(int priority) {
    if (priority == PRIORITY_EMERGENCY) return "Emergency";
    if (priority == PRIORITY_HIGH) return "High";
    if (priority == PRIORITY_NORMAL) return "Normal";
    if (priority == PRIORITY_LOW) return "Low";
    return "Unknown";
}

std::string currentTimestamp() {
    time_t now = time(0);
    struct tm* parts = localtime(&now);
    char buffer[32];
    strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M", parts);
    return std::string(buffer);
}

std::string makeRequestId(int number) {
    std::string digits = intToString(number);
    while (digits.size() < 4) {
        digits = "0" + digits;          // 7 -> 0007
    }
    return "REQ" + digits;
}

bool requestComesBefore(const Request& a, const Request& b) {
    if (a.priority != b.priority) return a.priority < b.priority;
    int byTime = a.time.compare(b.time);
    if (byTime != 0) return byTime < 0;
    return a.id.compare(b.id) < 0;      // IDs are zero-padded, so text order = number order
}

void printRequestHeader() {
    std::cout << std::left
              << std::setw(9) << "ID"
              << std::setw(14) << "User"
              << std::setw(10) << "Resource"
              << std::setw(18) << "Submitted"
              << std::setw(13) << "Priority"
              << "Status\n";
    std::cout << std::string(72, '-') << "\n";
}

void printRequestRow(const Request& r) {
    std::string priorityText = intToString(r.priority) + "-" + priorityName(r.priority);
    std::cout << std::left
              << std::setw(9) << r.id
              << std::setw(14) << r.user
              << std::setw(10) << r.resourceId
              << std::setw(18) << r.time
              << std::setw(13) << priorityText
              << r.status << "\n";
}

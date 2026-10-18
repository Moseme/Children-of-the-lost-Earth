#include "Utils.h"
#include <iostream>
#include <iomanip>
#include <sstream>

std::string trim(const std::string& s) {
    int start = 0;
    int end = (int)s.size() - 1;
    while (start <= end && (s[start] == ' ' || s[start] == '\t' || s[start] == '\r' || s[start] == '\n')) start++;
    while (end >= start && (s[end] == ' ' || s[end] == '\t' || s[end] == '\r' || s[end] == '\n')) end--;
    if (start > end) return "";
    return s.substr(start, end - start + 1);
}

bool parseInt(const std::string& s, int& value) {
    if (s.empty()) return false;
    int i = 0;
    bool negative = false;
    if (s[0] == '-') { negative = true; i = 1; }
    if (i >= (int)s.size()) return false;       // just a "-" sign
    if ((int)s.size() - i > 9) return false;    // too long, would overflow an int
    int result = 0;
    for (; i < (int)s.size(); i++) {
        if (s[i] < '0' || s[i] > '9') return false;
        result = result * 10 + (s[i] - '0');
    }
    value = negative ? -result : result;
    return true;
}

std::string readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string line;
    if (!std::getline(std::cin, line)) return "";
    return trim(line);
}

int readInt(const std::string& prompt, int minValue, int maxValue) {
    while (true) {
        std::cout << prompt;
        std::string line;
        if (!std::getline(std::cin, line)) return minValue;   // end of input
        int value;
        if (parseInt(trim(line), value) && value >= minValue && value <= maxValue) {
            return value;
        }
        std::cout << "Invalid input. Please enter a whole number from "
                  << minValue << " to " << maxValue << ".\n";
    }
}

int splitLine(const std::string& line, char separator, std::string fields[], int maxFields) {
    int count = 0;
    std::string current = "";
    for (int i = 0; i < (int)line.size(); i++) {
        if (line[i] == separator && count < maxFields - 1) {
            fields[count] = current;
            count++;
            current = "";
        } else {
            current += line[i];
        }
    }
    fields[count] = current;
    count++;
    return count;
}

void printResourceHeader() {
    std::cout << std::left
              << std::setw(10) << "ID"
              << std::setw(26) << "Name"
              << std::setw(22) << "Type"
              << std::setw(18) << "Location"
              << std::setw(10) << "Capacity"
              << "Status\n";
    std::cout << std::string(96, '-') << "\n";
}

void printResourceRow(const Resource& r) {
    std::cout << std::left
              << std::setw(10) << r.id
              << std::setw(26) << r.name
              << std::setw(22) << r.type
              << std::setw(18) << r.location
              << std::setw(10) << r.capacity
              << (r.available ? "Available" : "Unavailable") << "\n";
}

std::string intToString(int value) {
    std::ostringstream out;
    out << value;
    return out.str();
}

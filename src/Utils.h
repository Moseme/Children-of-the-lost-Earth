// SHARED HELPERS: safe keyboard input, text parsing, and printing of resources.
// Everyone can use these so the whole program behaves the same way.
#ifndef UTILS_H
#define UTILS_H
#include <string>
#include "Resource.h"

// Remove spaces, tabs and '\r' from both ends of a string.
std::string trim(const std::string& s);

// Convert a whole number to text (plain replacement for std::to_string).
std::string intToString(int value);

// Convert text to an int. Returns false if the text is not a whole number.
bool parseInt(const std::string& s, int& value);

// Print a prompt and read one whole line. Returns "" at end of input.
std::string readLine(const std::string& prompt);

// Keep asking until the user types a whole number between minValue and maxValue.
// At end of input it returns minValue (so loops that use 0 as "back" always finish).
int readInt(const std::string& prompt, int minValue, int maxValue);

// Split "a|b|c" into fields[]. Returns how many fields were found.
// At most maxFields fields are stored; the last one keeps any extra text.
int splitLine(const std::string& line, char separator, std::string fields[], int maxFields);

// Print resources as a neat table.
void printResourceHeader();
void printResourceRow(const Resource& r);

#endif

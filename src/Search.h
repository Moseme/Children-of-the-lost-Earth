// OWNER: Student A.
// Two searching algorithms on an ARRAY of Resource objects.
//   linearSearchById : checks items one by one.        O(n). Works on unsorted data.
//   binarySearchById : halves the search range.        O(log n). Array MUST be sorted by ID.
// Both return the index of the match, or -1 if not found, and count the comparisons made.
#ifndef SEARCH_H
#define SEARCH_H
#include <string>
#include "Resource.h"

int linearSearchById(const Resource arr[], int n, const std::string& id, long& comparisons);
int binarySearchById(const Resource arr[], int n, const std::string& id, long& comparisons);

#endif

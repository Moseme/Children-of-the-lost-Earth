// OWNER: Student B.
// Sorting requests by submission time, priority or status, using MERGE SORT
// (same idea as Student A's resource merge sort): O(n log n) in every case.
#ifndef REQUESTSORT_H
#define REQUESTSORT_H
#include "Request.h"

enum RequestSortCriterion {
    REQSORT_BY_TIME = 1,
    REQSORT_BY_PRIORITY = 2,
    REQSORT_BY_STATUS = 3
};

const char* requestCriterionName(int criterion);
int compareRequests(const Request& a, const Request& b, int criterion);
void mergeSortRequests(Request arr[], int n, int criterion, long& comparisons);

#endif

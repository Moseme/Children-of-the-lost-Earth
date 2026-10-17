// OWNER: Student A.
// Two sorting algorithms that work on a dynamic ARRAY of Resource objects.
//   insertionSort : simple, O(n^2) average and worst case, O(n) best case (already sorted)
//   mergeSort     : divide and conquer, O(n log n) in best, average AND worst case,
//                   needs O(n) extra memory
// Both count how many comparisons they make, so the algorithms can be compared with real numbers.
#ifndef SORTING_H
#define SORTING_H
#include "Resource.h"

enum SortCriterion {
    SORT_BY_ID = 1,
    SORT_BY_NAME = 2,
    SORT_BY_CAPACITY = 3,
    SORT_BY_TYPE = 4
};

const char* criterionName(int criterion);

// Returns a negative number if a comes before b, 0 if equal, a positive number if a comes after b.
int compareResources(const Resource& a, const Resource& b, int criterion);

void insertionSort(Resource arr[], int n, int criterion, long& comparisons);
void mergeSort(Resource arr[], int n, int criterion, long& comparisons);

#endif

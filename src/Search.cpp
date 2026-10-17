#include "Search.h"

int linearSearchById(const Resource arr[], int n, const std::string& id, long& comparisons) {
    for (int i = 0; i < n; i++) {
        comparisons++;
        if (arr[i].id == id) {
            return i;
        }
    }
    return -1;
}

int binarySearchById(const Resource arr[], int n, const std::string& id, long& comparisons) {
    int low = 0;
    int high = n - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        comparisons++;
        int result = id.compare(arr[mid].id);   // same ordering that SORT_BY_ID uses
        if (result == 0) {
            return mid;                 // found
        } else if (result < 0) {
            high = mid - 1;             // the ID must be in the left half
        } else {
            low = mid + 1;              // the ID must be in the right half
        }
    }
    return -1;                          // not found
}

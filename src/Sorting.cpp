#include "Sorting.h"

const char* criterionName(int criterion) {
    if (criterion == SORT_BY_ID) return "Resource ID";
    if (criterion == SORT_BY_NAME) return "Name";
    if (criterion == SORT_BY_CAPACITY) return "Capacity";
    if (criterion == SORT_BY_TYPE) return "Type";
    return "Unknown";
}

int compareResources(const Resource& a, const Resource& b, int criterion) {
    if (criterion == SORT_BY_NAME) return a.name.compare(b.name);
    if (criterion == SORT_BY_CAPACITY) return a.capacity - b.capacity;
    if (criterion == SORT_BY_TYPE) return a.type.compare(b.type);
    return a.id.compare(b.id);   // default: by ID
}

// ---------------------------------------------------------------
// INSERTION SORT
// Idea: the left part of the array is always sorted. Take the next item
// ("key") and slide it left until it sits in the right place.
// Best case O(n), average and worst case O(n^2). Space O(1).
// ---------------------------------------------------------------
void insertionSort(Resource arr[], int n, int criterion, long& comparisons) {
    for (int i = 1; i < n; i++) {
        Resource key = arr[i];
        int j = i - 1;
        while (j >= 0) {
            comparisons++;
            if (compareResources(arr[j], key, criterion) > 0) {
                arr[j + 1] = arr[j];     // shift the bigger item one place right
                j--;
            } else {
                break;                   // found the correct place
            }
        }
        arr[j + 1] = key;
    }
}

// ---------------------------------------------------------------
// MERGE SORT
// Idea: split the array in half, sort each half (recursively), then
// MERGE the two sorted halves into one sorted array.
// Always O(n log n). Needs a temporary array, so space is O(n).
// ---------------------------------------------------------------

// Merge arr[left..mid] and arr[mid+1..right], which are both already sorted.
static void merge(Resource arr[], Resource temp[], int left, int mid, int right,
                  int criterion, long& comparisons) {
    int i = left;        // reads the left half
    int j = mid + 1;     // reads the right half
    int k = left;        // writes into temp

    while (i <= mid && j <= right) {
        comparisons++;
        if (compareResources(arr[i], arr[j], criterion) <= 0) {   // <= keeps equal items in original order
            temp[k] = arr[i];
            i++;
        } else {
            temp[k] = arr[j];
            j++;
        }
        k++;
    }
    while (i <= mid) { temp[k] = arr[i]; i++; k++; }      // leftovers from the left half
    while (j <= right) { temp[k] = arr[j]; j++; k++; }    // leftovers from the right half

    for (int x = left; x <= right; x++) {                 // copy the merged result back
        arr[x] = temp[x];
    }
}

static void mergeSortRecursive(Resource arr[], Resource temp[], int left, int right,
                               int criterion, long& comparisons) {
    if (left >= right) {
        return;                       // 0 or 1 item is already sorted
    }
    int mid = left + (right - left) / 2;
    mergeSortRecursive(arr, temp, left, mid, criterion, comparisons);
    mergeSortRecursive(arr, temp, mid + 1, right, criterion, comparisons);
    merge(arr, temp, left, mid, right, criterion, comparisons);
}

void mergeSort(Resource arr[], int n, int criterion, long& comparisons) {
    if (n <= 1) {
        return;
    }
    Resource* temp = new Resource[n];     // one temporary array, reused by every merge
    mergeSortRecursive(arr, temp, 0, n - 1, criterion, comparisons);
    delete[] temp;
}

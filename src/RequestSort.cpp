#include "RequestSort.h"

const char* requestCriterionName(int criterion) {
    if (criterion == REQSORT_BY_TIME) return "Submission time";
    if (criterion == REQSORT_BY_PRIORITY) return "Priority";
    if (criterion == REQSORT_BY_STATUS) return "Status";
    return "Unknown";
}

// Negative: a first. Zero: equal. Positive: b first. Ties fall back to ID so the result is stable.
int compareRequests(const Request& a, const Request& b, int criterion) {
    int result = 0;
    if (criterion == REQSORT_BY_PRIORITY) {
        result = a.priority - b.priority;
        if (result == 0) result = a.time.compare(b.time);
    } else if (criterion == REQSORT_BY_STATUS) {
        result = a.status.compare(b.status);
        if (result == 0) result = a.time.compare(b.time);
    } else {
        result = a.time.compare(b.time);
    }
    if (result == 0) result = a.id.compare(b.id);
    return result;
}

static void mergeParts(Request arr[], Request temp[], int left, int mid, int right,
                       int criterion, long& comparisons) {
    int i = left;
    int j = mid + 1;
    int k = left;
    while (i <= mid && j <= right) {
        comparisons++;
        if (compareRequests(arr[i], arr[j], criterion) <= 0) {
            temp[k] = arr[i];
            i++;
        } else {
            temp[k] = arr[j];
            j++;
        }
        k++;
    }
    while (i <= mid) { temp[k] = arr[i]; i++; k++; }
    while (j <= right) { temp[k] = arr[j]; j++; k++; }
    for (int x = left; x <= right; x++) {
        arr[x] = temp[x];
    }
}

static void mergeSortPart(Request arr[], Request temp[], int left, int right,
                          int criterion, long& comparisons) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    mergeSortPart(arr, temp, left, mid, criterion, comparisons);
    mergeSortPart(arr, temp, mid + 1, right, criterion, comparisons);
    mergeParts(arr, temp, left, mid, right, criterion, comparisons);
}

void mergeSortRequests(Request arr[], int n, int criterion, long& comparisons) {
    if (n <= 1) return;
    Request* temp = new Request[n];
    mergeSortPart(arr, temp, 0, n - 1, criterion, comparisons);
    delete[] temp;
}

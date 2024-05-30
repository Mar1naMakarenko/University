#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>

using namespace std;
using namespace std::chrono;

void selectionSort(vector<system_clock::time_point>& arr);
void quicksort(vector<system_clock::time_point>& arr, int low, int high);
void mergeSort(vector<system_clock::time_point>& arr, int l, int r);
void combinedSort(vector<system_clock::time_point>& arr, int low, int high);
void librarySort(vector<system_clock::time_point>& arr);
int partition(vector<system_clock::time_point>& arr, int low, int high);
void merge(vector<system_clock::time_point>& arr, int l, int m, int r);

void selectionSort(vector<system_clock::time_point>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        int minIdx = i;
        for (int j = i + 1; j < n; ++j) {
            if (arr[j] < arr[minIdx]) {
                minIdx = j;
            }
        }
        swap(arr[i], arr[minIdx]);
    }
}

void quicksort(vector<system_clock::time_point>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quicksort(arr, low, pi);
        quicksort(arr, pi + 1, high);
    }
}

int partition(vector<system_clock::time_point>& arr, int low, int high) {
    system_clock::time_point pivot = arr[low];
    int i = low - 1;
    int j = high + 1;
    while (true) {
        do {
            ++i;
        } while (arr[i] < pivot);

        do {
            --j;
        } while (arr[j] > pivot);

        if (i >= j) {
            return j;
        }
        swap(arr[i], arr[j]);
    }
}

void mergeSort(vector<system_clock::time_point>& arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

void merge(vector<system_clock::time_point>& arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    vector<system_clock::time_point> L(n1), R(n2);
    for (int i = 0; i < n1; ++i) {
        L[i] = arr[l + i];
    }
    for (int j = 0; j < n2; ++j) {
        R[j] = arr[m + 1 + j];
    }

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }

    while (i < n1) {
        arr[k++] = L[i++];
    }
    while (j < n2) {
        arr[k++] = R[j++];
    }
}

void combinedSort(vector<system_clock::time_point>& arr, int low, int high) {
    if (high - low + 1 > 10) {
        quicksort(arr, low, high);
    } else {
        selectionSort(arr);
    }
}

void librarySort(vector<system_clock::time_point>& arr) {
    sort(arr.begin(), arr.end());
}

vector<system_clock::time_point> generateRandomDates(int size) {
    vector<system_clock::time_point> randomDates;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 1e9);

    for (int i = 0; i < size; ++i) {
        system_clock::time_point randomDate = system_clock::time_point(seconds(dis(gen)));
        randomDates.push_back(randomDate);
    }
    return randomDates;
}

void demonstrationMode() {
    int size = 10;
    vector<system_clock::time_point> randomDates = generateRandomDates(size);

    cout << "Unsorted array: ";
    for (const auto& date : randomDates) {
        cout << date.time_since_epoch().count() << " ";
    }
    cout << endl;

    cout << "Selection Sort:" << endl;
    vector<system_clock::time_point> selectionSorted = randomDates;
    selectionSort(selectionSorted);
    for (const auto& date : selectionSorted) {
        cout << date.time_since_epoch().count() << " ";
    }
    cout << endl;

    cout << "Quicksort:" << endl;
    vector<system_clock::time_point> quickSorted = randomDates;
    quicksort(quickSorted, 0, quickSorted.size() - 1);
    for (const auto& date : quickSorted) {
        cout << date.time_since_epoch().count() << " ";
    }
    cout << endl;

    cout << "Merge Sort:" << endl;
    vector<system_clock::time_point> mergeSorted = randomDates;
    mergeSort(mergeSorted, 0, mergeSorted.size() - 1);
    for (const auto& date : mergeSorted) {
        cout << date.time_since_epoch().count() << " ";
    }
    cout << endl;

    cout << "Combined Sort:" << endl;
    vector<system_clock::time_point> combinedSorted = randomDates;
    combinedSort(combinedSorted, 0, combinedSorted.size() - 1);
    for (const auto& date : combinedSorted) {
        cout << date.time_since_epoch().count() << " ";
    }
    cout << endl;

    cout << "Library Sort:" << endl;
    vector<system_clock::time_point> librarySorted = randomDates;
    librarySort(librarySorted);
    for (const auto& date : librarySorted) {
        cout << date.time_since_epoch().count() << " ";
    }
    cout << endl;
}

void benchmarkMode() {
    int size = 10000;
    vector<system_clock::time_point> randomDates = generateRandomDates(size);

    auto start = high_resolution_clock::now();
    selectionSort(randomDates);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Selection Sort Execution Time: " << duration.count() << " microseconds" << endl;

    start = high_resolution_clock::now();
    quicksort(randomDates, 0, randomDates.size() - 1);
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    cout << "Quicksort Execution Time: " << duration.count() << " microseconds" << endl;

    start = high_resolution_clock::now();
    mergeSort(randomDates, 0, randomDates.size() - 1);
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    cout << "Merge Sort Execution Time: "
            << duration.count() << " microseconds" << endl;

    start = high_resolution_clock::now();
    combinedSort(randomDates, 0, randomDates.size() - 1);
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    cout << "Combined Sort Execution Time: " << duration.count() << " microseconds" << endl;

    start = high_resolution_clock::now();
    librarySort(randomDates);
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    cout << "Library Sort Execution Time: " << duration.count() << " microseconds" << endl;
}

int main() {
    cout << "Demonstration Mode:" << endl;
    demonstrationMode();

    cout << "\nBenchmark Mode:" << endl;
    benchmarkMode();

    return 0;
}

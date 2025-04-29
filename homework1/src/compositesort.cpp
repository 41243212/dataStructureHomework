#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <random>
#include <chrono>

// Thresholds
const int INSERTION_SORT_THRESHOLD = 16;

std::vector<int> generateRandomArray(int n) {
    std::vector<int> arr;
    arr.reserve(n);

    std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr)));
    std::uniform_int_distribution<int> dist(0, n * 2);

    for (int i = 0; i < n; ++i) {
        arr.push_back(dist(rng));
    }
    return arr;
}

// Insertion sort
void insertionSort(std::vector<int>& arr, int left, int right) {
    for (int i = left + 1; i <= right; ++i) {
        int key = arr[i];
        int j = i - 1;
        while (j >= left && arr[j] > key) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}

// Heap sort (used for recursion fallback)
void heapify(std::vector<int>& arr, int n, int i, int offset) {
    int largest = i;
    int l = 2 * i + 1 - offset;
    int r = 2 * i + 2 - offset;

    if (l < n && arr[l] > arr[largest]) largest = l;
    if (r < n && arr[r] > arr[largest]) largest = r;

    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        heapify(arr, n, largest, offset);
    }
}

void heapSort(std::vector<int>& arr, int left, int right) {
    int n = right - left + 1;
    for (int i = left + n / 2 - 1; i >= left; --i)
        heapify(arr, left + n, i, left);
    for (int i = right; i > left; --i) {
        std::swap(arr[left], arr[i]);
        heapify(arr, i, left, left);
    }
}

// Merge sort
void merge(std::vector<int>& arr, int l, int m, int r) {
    std::vector<int> left(arr.begin() + l, arr.begin() + m + 1);
    std::vector<int> right(arr.begin() + m + 1, arr.begin() + r + 1);

    int i = 0, j = 0, k = l;
    while (i < left.size() && j < right.size())
        arr[k++] = (left[i] <= right[j]) ? left[i++] : right[j++];
    while (i < left.size()) arr[k++] = left[i++];
    while (j < right.size()) arr[k++] = right[j++];
}

void mergeSort(std::vector<int>& arr, int l, int r) {
    if (r - l <= INSERTION_SORT_THRESHOLD) {
        insertionSort(arr, l, r);
        return;
    }
    int m = l + (r - l) / 2;
    mergeSort(arr, l, m);
    mergeSort(arr, m + 1, r);
    merge(arr, l, m, r);
}

int medianOfThree(std::vector<int>& arr, int low, int high) {
    int mid = low + (high - low) / 2;
    if (arr[low] > arr[mid]) std::swap(arr[low], arr[mid]);
    if (arr[low] > arr[high]) std::swap(arr[low], arr[high]);
    if (arr[mid] > arr[high]) std::swap(arr[mid], arr[high]);

    // Place the pivot at the right position by swapping with high
    std::swap(arr[mid], arr[high]);
    return arr[high];
}

// Composite quicksort + fallback (like introsort)
void quickSort(std::vector<int>& arr, int left, int right, int depthLimit) {
    if (right - left <= INSERTION_SORT_THRESHOLD) {
        insertionSort(arr, left, right);
        return;
    }

    if (depthLimit == 0) {
        heapSort(arr, left, right);
        return;
    }

	int pivot = medianOfThree(arr, left, right);
    int i = left, j = right;
    while (i <= j) {
        while (arr[i] < pivot) ++i;
        while (arr[j] > pivot) --j;
        if (i <= j) std::swap(arr[i++], arr[j--]);
    }

    if (left < j) quickSort(arr, left, j, depthLimit - 1);
    if (i < right) quickSort(arr, i, right, depthLimit - 1);
}

// Entry point
void compositeSort(std::vector<int>& arr) {
    // detect "nearly sorted" data for merge sort
    int count_sorted = 0;
    for (size_t i = 1; i < arr.size(); ++i)
        if (arr[i - 1] <= arr[i]) ++count_sorted;
    double sorted_ratio = (double)count_sorted / (arr.size() - 1);

    if (sorted_ratio > 0.95) {
        // Use stable merge sort if already nearly sorted
        mergeSort(arr, 0, arr.size() - 1);
    } else {
        int depthLimit = 2 * log(arr.size());
        quickSort(arr, 0, arr.size() - 1, depthLimit);
    }
}

int main()
{
    int sizes[] = { 500, 1000, 2000, 3000, 4000, 5000, 10000, 100000 };
	std::cout << "Composite Sort Performance Test\n";
    for (int i = 0; i < 8; i++) {
        int tries = 1000;
        int size = sizes[i];
        std::cout << "n = " << size << std::endl;
        std::chrono::duration<double> elapsed = std::chrono::duration<double>::zero();
        for (int j = 0; j < tries; j++) {
            std::vector<int> result = generateRandomArray(size);
            auto start = std::chrono::high_resolution_clock::now();
            compositeSort(result);
            auto end = std::chrono::high_resolution_clock::now();
            elapsed += end - start;
        }
        elapsed /= tries;
        std::cout << "Avg execution time: " << elapsed.count() * 1000 << " milliseconds\n";
    }
	std::cout << "\nstd::sort Performance Test\n";
	for (int i = 0; i < 8; i++) {
		int tries = 1000;
		int size = sizes[i];
		std::cout << "n = " << size << std::endl;
		std::chrono::duration<double> elapsed = std::chrono::duration<double>::zero();
		for (int j = 0; j < tries; j++) {
			std::vector<int> result = generateRandomArray(size);
			auto start = std::chrono::high_resolution_clock::now();
			std::sort(result.begin(), result.end());
			auto end = std::chrono::high_resolution_clock::now();
			elapsed += end - start;
		}
		elapsed /= tries;
		std::cout << "Avg execution time: " << elapsed.count() * 1000 << " milliseconds\n";
	}
    return 0;
}
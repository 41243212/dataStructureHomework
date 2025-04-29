#include <iostream>
#include <algorithm>
#include <chrono>
#include <vector>
#include <random>

size_t currentStackBytes = 0;
size_t peakStackBytes = 0;

// Helper to track function call stack memory usage
void addStackFrame(size_t size) {
    currentStackBytes += size;
    if (currentStackBytes > peakStackBytes)
        peakStackBytes = currentStackBytes;
}

void removeStackFrame(size_t size) {
    currentStackBytes -= size;
}

// Estimate the size of each function's stack frame
constexpr size_t sizeOfQuickSortFrame = sizeof(int) * 3 + sizeof(std::vector<int>*); // low, high, pivotIndex, arr reference
constexpr size_t sizeOfPartitionFrame = sizeof(int) * 3 + sizeof(std::vector<int>*); // pivot, next, i, arr reference
constexpr size_t sizeOfMedianOfThreeFrame = sizeof(int) * 4 + sizeof(std::vector<int>*); // low, high, mid, arr reference

// Function to find the median of three elements and use it as pivot
int medianOfThree(std::vector<int>& arr, int low, int high) {
    addStackFrame(sizeOfMedianOfThreeFrame);
    int mid = low + (high - low) / 2;
    if (arr[low] > arr[mid]) std::swap(arr[low], arr[mid]);
    if (arr[low] > arr[high]) std::swap(arr[low], arr[high]);
    if (arr[mid] > arr[high]) std::swap(arr[mid], arr[high]);

    // Place the pivot at the right position by swapping with high
    std::swap(arr[mid], arr[high]);
    removeStackFrame(sizeOfMedianOfThreeFrame);
    return arr[high];
}

// Function to generate a bad input for quicksort
void createBadInput(std::vector<int>& arr, int start, int end, int& current) {
    if (start > end) return;

    int mid = (start + end) / 2;
    arr[mid] = current++;

    // Recursively fill left and right to maintain bad pivot conditions
    createBadInput(arr, start, mid - 1, current);
    createBadInput(arr, mid + 1, end, current);
}

std::vector<int> generateQuickSortWorst(int n) {
    std::vector<int> arr(n);
    int current = 1;
    createBadInput(arr, 0, n - 1, current);
    return arr;
}

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

// Partition function
int partition(std::vector<int>& arr, int low, int high) {
    addStackFrame(sizeOfPartitionFrame);
    int pivot = medianOfThree(arr, low, high);
    int next = low;

    for (int i = low; i < high; i++) {
        if (arr[i] < pivot) {
			std::swap(arr[i], arr[next]);
			next++;
		}
    }
    std::swap(arr[next], arr[high]); // Restore pivot
    removeStackFrame(sizeOfPartitionFrame);
    return next;
}

// QuickSort function
void quickSort(std::vector<int>& arr, int low, int high) {
    addStackFrame(sizeOfQuickSortFrame);
    if (low < high) {
        int pivotIndex = partition(arr, low, high);
        quickSort(arr, low, pivotIndex - 1);
        quickSort(arr, pivotIndex + 1, high);
    }
    removeStackFrame(sizeOfQuickSortFrame);
}

// Main function
int main() {
    int sizes[] = { 500, 1000, 2000, 3000, 4000, 5000 };

	for (int i = 0; i < 6; i++) {
		int size = sizes[i];
		std::vector<int> result = generateQuickSortWorst(size);
        std::cout << "n = " << size << std::endl;
		auto start = std::chrono::high_resolution_clock::now();
		quickSort(result, 0, size - 1);
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed = end - start;
		std::cout << "Execution time: " << elapsed.count() * 1000 << " milliseconds\n";
        std::cout << "Peak Stack Memory Usage During Sorting: " << peakStackBytes << " bytes\n";
	}

    std::cout << "Average case\n";
    for (int i = 0; i < 6; i++) {
        int tries = 1000;
        int size = sizes[i];
        std::cout << "n = " << size << std::endl;
        std::chrono::duration<double> elapsed = std::chrono::duration<double>::zero();
        for (int j = 0; j < tries; j++) {
            std::vector<int> result = generateRandomArray(size);
            auto start = std::chrono::high_resolution_clock::now();
            quickSort(result, 0, size - 1);
            auto end = std::chrono::high_resolution_clock::now();
            elapsed += end - start;
        }
        elapsed /= tries;
        std::cout << "Avg execution time: " << elapsed.count() * 1000 << " milliseconds\n";
    }
    return 0;
}

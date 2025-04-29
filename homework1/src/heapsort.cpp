#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <chrono>
#include <cmath>
#include <fstream>
#include <string>
#include <algorithm>

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

size_t totalMemoryUsed = 0;

void heapify(std::vector<int>& arr, int arraySize, int i) {
	int left = 2 * i + 1; // Left child
	int right = 2 * i + 2; // Right child
	int big = i; // Assume parent is largest

	if (right < arraySize && arr[right] > arr[big]) {
		big = right;
	}
	if (left < arraySize && arr[left] > arr[big]) {
		big = left;
	}

	if (big != i) {
		std::swap(arr[i], arr[big]);
		heapify(arr, arraySize, big); // Recursive call
	}
}

void buildMaxHeap(std::vector<int>& arr) {
	int n = static_cast<int>(arr.size());
	for (int i = n / 2 - 1; i >= 0; --i) {
		heapify(arr, n, i);
	}
}

void heapSort(std::vector<int>& arr) {
	int n = static_cast<int>(arr.size());
	buildMaxHeap(arr);

	for (int heapSize = n; heapSize > 1; --heapSize) {
		std::swap(arr[0], arr[heapSize - 1]);
		heapify(arr, heapSize - 1, 0);
	}

	// Calculate peak memory usage after sorting
	// Stack space: 3 ints (left, right, big) per stack frame, max depth = floor(log2(n)) + 1
	// Local variables: n in heapSort and buildMaxHeap (2 * sizeof(int))
	size_t maxStackDepth = static_cast<size_t>(std::floor(std::log2(n)) + 1);
	totalMemoryUsed = (maxStackDepth * 3 * sizeof(int)) + (2 * sizeof(int));
}

int generateWorst() {
	int sizes[] = { 500, 1000, 2000, 3000, 4000, 5000 };
	int n; // Size of the array
	int tries = 1000; // Number of random inputs
	int reruns = 5;   // Number of reruns for each input to average the time

	// Variables to track the slowest input and its execution time
	double maxAvgTime = 0;
	std::vector<int> slowestInput;
	std::vector<int> slowestOutput;
	size_t maxTotalMemoryUsed = 0;
	for (int k = 0; k < 6; k++) {
		n = sizes[k];
		for (int i = 0; i < tries; ++i) {
			std::vector<int> input = generateRandomArray(n);
			std::vector<int> copy;
			double totalTime = 0.0;

			// Run multiple trials on the same input to average the time
			for (int j = 0; j < reruns; ++j) {
				copy = input;
				totalMemoryUsed = 0;
				// Measure the execution time using chrono
				auto start = std::chrono::high_resolution_clock::now();
				heapSort(copy);
				auto end = std::chrono::high_resolution_clock::now();

				// Calculate the elapsed time in milliseconds and accumulate
				std::chrono::duration<double> elapsed = end - start;
				totalTime += elapsed.count() * 1000.0; // Convert to milliseconds
			}

			// Average time for this input
			double avgTime = totalTime / reruns;

			// Track the slowest input based on average time
			if (avgTime > maxAvgTime) {
				maxAvgTime = avgTime;
				slowestInput = input;
				slowestOutput = copy;
				maxTotalMemoryUsed = totalMemoryUsed;
			}
		}

		std::cout << "n=" << n << "\nWorst execution time : " << maxAvgTime << " ms\n";
		std::cout << "Estimated total memory used: " << maxTotalMemoryUsed << " bytes\n";
		// Writing input of the slowest input to file
		std::string filename = "heapSort_n" + std::to_string(n) + "_in.txt";
		std::ofstream outputFile(filename);
		if (outputFile.is_open()) {
			outputFile << n << std::endl; // Write the size of the array
			for (int value : slowestInput) {
				outputFile << value << " "; // Write to file
			}
			outputFile.close();
			outputFile.clear();
		} else {
			std::cerr << "Error: Unable to open file for writing." << std::endl;
			return 1;
		}
		// Writing output of the slowest input to file
		filename = "heapSort_n" + std::to_string(n) + "_out.txt";
		outputFile.open(filename);
		if (outputFile.is_open()) {
			outputFile << n << std::endl; // Write the size of the array
			for (int value : slowestOutput) {
				outputFile << value << " "; // Write to file
			}
			outputFile.close();
		} else {
			std::cerr << "Error: Unable to open file for writing." << std::endl;
			return 1;
		}
	}
}

void avgCase() {
	int sizes[] = { 500, 1000, 2000, 3000, 4000, 5000 };
	for (int i = 0; i < 6; i++) {
		int tries = 1000;
		int size = sizes[i];
		std::cout << "n = " << size << std::endl;
		std::chrono::duration<double> elapsed = std::chrono::duration<double>::zero();
		for (int j = 0; j < tries; j++) {
			std::vector<int> result = generateRandomArray(size);
			auto start = std::chrono::high_resolution_clock::now();
			heapSort(result);
			auto end = std::chrono::high_resolution_clock::now();
			elapsed += end - start;
		}
		elapsed /= tries;
		std::cout << "Avg execution time: " << elapsed.count() * 1000 << " milliseconds\n";
	}
}

int main() {
    generateWorst();
	std::cout << "Average case\n";
	avgCase();
    return 0;
}
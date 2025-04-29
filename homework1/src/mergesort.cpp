#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <cmath>
#include <fstream>
#include <string>

std::vector<int> generateRandomArray(int n) {
    std::vector<int> arr;
    arr.reserve(n);

    // Initialize Mersenne Twister engine with time-based seed
    std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr)));
    std::uniform_int_distribution<int> dist(0, n*3);

    for (int i = 0; i < n; ++i) {
        arr.push_back(dist(rng));
    }

    return arr;
}

size_t totalMemoryUsed = 0;

template <class T>
void Merge(std::vector<T>& initList, std::vector<T>& mergedList, const int left, const int mid, const int n) {
	int i1 = left, i2 = mid + 1, iResult = left;
	while (i1 <= mid && i2 <= n) {
		if (initList[i1] <= initList[i2]) {
			mergedList[iResult++] = initList[i1++];
		} else {
			mergedList[iResult++] = initList[i2++];
		}
	}

	// copy remaining records, if any
	std::copy(initList.begin() + i1, initList.begin() + mid + 1, mergedList.begin() + iResult);
	iResult += mid - i1 + 1;
	std::copy(initList.begin() + i2, initList.begin() + n + 1, mergedList.begin() + iResult);
}

template <class T>
void MergePass(std::vector<T>& initList, std::vector<T>& resultList, const int n, const int s) {
	int i = 0;
	while (i <= n - 2 * s) {
		Merge(initList, resultList, i, i + s - 1, i + 2 * s - 1);
		i += 2 * s;
	}
	if (i + s < n) {
		Merge(initList, resultList, i, i + s - 1, n - 1);
	} else {
		std::copy(initList.begin() + i, initList.begin() + n, resultList.begin() + i);
	}
}

template <class T>
void MergeSort(std::vector<T>& a, const int n) {
	std::vector<T> temp(n + 1);
	totalMemoryUsed += (n + 1) * sizeof(T);
	for (int l = 1; l < n; l *= 2) {
		MergePass(a, temp, n, l);
		l *= 2;
		MergePass(temp, a, n, l);
	}
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
				MergeSort(copy, n);
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
		std::string filename = "mergeSort_n" + std::to_string(n) + "_in.txt";
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
		filename = "mergeSort_n" + std::to_string(n) + "_out.txt";
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
			MergeSort(result, size);
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
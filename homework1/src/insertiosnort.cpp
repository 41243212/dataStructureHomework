#include <iostream>
#include <algorithm>
#include <chrono>
#include <vector>
#include <random>
#include <ctime>

// Generate [n, n-1, n-2, ... , 1]
std::vector<int> generateInsertionSortWorst(int n) {
	std::vector<int> arr(n);
	for (std::vector<int>::iterator it = arr.begin(); it != arr.end(); it++)
	{
		*it = n - (it - arr.begin());
	}
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

void insertionSort(std::vector<int>& arr)
{
    int size = arr.size();
    for (int i = 1; i < size; i++)
    {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

int main()
{
    int sizes[] = {500, 1000, 2000, 3000, 4000, 5000};

    for (int i = 0; i < 6; i++) {
        int size = sizes[i];
        std::vector<int> result = generateInsertionSortWorst(size);
        std::cout << "n = " << size;
        if (size <= 1000) {
            std::cout << ", Input array: ";
            for (int value : result) {
                std::cout << value << " ";
            }
        }
        std::cout << std::endl;
        auto start = std::chrono::high_resolution_clock::now();
        insertionSort(result);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << "Execution time: " << elapsed.count() * 1000 << " milliseconds\n";
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
            insertionSort(result);
            auto end = std::chrono::high_resolution_clock::now();
            elapsed += end - start;
        }
		elapsed /= tries;
        std::cout << "Avg execution time: " << elapsed.count() * 1000 << " milliseconds\n";
    }
    
    return 0;
}
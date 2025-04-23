#include <iostream>
#include <algorithm>
#include <chrono>
#include <vector>

// Generate [n, n-1, n-2, ... , 1]
std::vector<int> generateInsertionSortWorst(int n) {
	std::vector<int> arr(n);
	for (std::vector<int>::iterator it = arr.begin(); it != arr.end(); it++)
	{
		*it = n - (it - arr.begin());
	}
	return arr;
}

void insertionSort(std::vector<int>& arr, int size)
{
    for (int i = 1; i <= size; i++)
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
        insertionSort(result, size - 1);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << "Execution time: " << elapsed.count() * 1000 << " milliseconds\n";
    }
    
    return 0;
}
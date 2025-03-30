#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>

// Function to find the median of three elements and use it as pivot
int medianOfThree(int arr[], int low, int high) {
    int mid = low + (high - low) / 2;
    if (arr[low] > arr[mid]) std::swap(arr[low], arr[mid]);
    if (arr[low] > arr[high]) std::swap(arr[low], arr[high]);
    if (arr[mid] > arr[high]) std::swap(arr[mid], arr[high]);

    // Place the pivot at the right position by swapping with high-1
    std::swap(arr[mid], arr[high]);
    return arr[high];
}

// Partition function
int partition(int arr[], int low, int high) {
    int pivot = medianOfThree(arr, low, high);
    int next = low;

    for (int i = low; i < high; i++) {
        if (arr[i] < pivot) {
			std::swap(arr[i], arr[next]);
			next++;
		}
    }
    std::swap(arr[next], arr[high]); // Restore pivot
    return next;
}

// QuickSort function
void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pivotIndex = partition(arr, low, high);
        quickSort(arr, low, pivotIndex - 1);
        quickSort(arr, pivotIndex + 1, high);
    }
}

// Utility function to print array
void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}

// Main function
int main() {
    int* resultArray = nullptr;
    int size = 0;
    std::ifstream inputFile("random.txt");
    if (inputFile.is_open()) {
        std::string line;
		std::getline(inputFile, line);
		size = std::stoi(line);
		resultArray = new int[size];
		std::getline(inputFile, line);
		std::istringstream stream(line);
		for (int i = 0; i < size; i++) {
			stream >> resultArray[i];
		}
		inputFile.close();
    } else {
		std::cerr << "Error: Unable to open file for reading." << std::endl;
		return 1;
	}

    quickSort(resultArray, 0, size - 1);

    std::cout << "Sorted array: ";
    printArray(resultArray, size);

    return 0;
}

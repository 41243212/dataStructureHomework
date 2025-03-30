#include <iostream>
#include <vector>
#include <random>
#include <fstream>

std::vector<int> generateRandomArray(int n, int i) {
    if (n <= 0 || i <= 0) {
        return {}; // Return empty vector for invalid input
    }

    std::vector<int> randomArray(n);

    // Seed the random number generator
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    for (int j = 0; j < n; ++j) {
        randomArray[j] = std::rand() % i + 1; // Range [1, i]
    }

    return randomArray;
}

int main() {
    int n, i;

    std::cout << "Enter the size of the array (n): ";
    std::cin >> n;

    std::cout << "Enter the upper bound of the range (i): ";
    std::cin >> i;

    std::vector<int> resultArray = generateRandomArray(n, i);

    if (!resultArray.empty()) {
        std::ofstream outputFile("random.txt"); // Open file for writing

        if (outputFile.is_open()) {
            outputFile << n << std::endl; // Write the size of the array
            for (int value : resultArray) {
                outputFile << value << " "; // Write to file
            }
            outputFile << std::endl;
            outputFile.close(); // Close the file
            std::cout << "Random array written to file" << std::endl;
        } else {
            std::cerr << "Error: Unable to open file for writing." << std::endl;
        }

    } else {
        std::cout << "Invalid input. Array not generated." << std::endl;
    }

    return 0;
}
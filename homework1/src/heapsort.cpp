#include <iostream>
#include <vector>
#include <random>
#include <ctime>

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

void heapify(std::vector<int>& arr, int arraySize, int i) {
    int left = 2 * i + 1;//子左節點
    int right = 2 * i + 2;//子右節點
    int big = i; //先假設為父節點

    //只要還有葉節點就比
    if (right < arraySize && arr[right] > arr[big]) {
        big = right;
    }
    if (left < arraySize && arr[left] > arr[big]) {
        big = left;
    }

    //如果big非最大
    if (big != i) {
        int temp = arr[i];
        arr[i] = arr[big];
        arr[big] = temp;
        heapify(arr, arraySize, big);//繼續排序直到完成
    }
}

// 建立最大堆：從最後一個非葉節點開始，逐一向下調整
void buildMaxHeap(std::vector<int>& arr) {
    int n = static_cast<int>(arr.size());
    for (int i = n / 2 - 1; i >= 0; --i) {
        heapify(arr, n, i);
    }
}

// 堆積排序主函式
void heapSort(std::vector<int>& arr) {
    int n = static_cast<int>(arr.size());
    buildMaxHeap(arr);//先建立最大堆

    for (int heapSize = n; heapSize > 1; --heapSize) {//把最大跟最後一個最小交換
        int temp = arr[0];
        arr[0] = arr[heapSize - 1];
        arr[heapSize - 1] = temp;

        heapify(arr, heapSize - 1, 0);//遞規排序
    }
}

int main() {
    int n;
    std::cout << "請輸入要多少數字";
    std::cin >> n;

    std::vector<int> data = generateRandomArray(n);

    std::cout << "排序前：";
    for (int v : data) std::cout << v << ' ';
    std::cout << '\n';

    heapSort(data);

    std::cout << "排序後：";
    for (int v : data) std::cout << v << ' ';
    std::cout << '\n';

    return 0;
}
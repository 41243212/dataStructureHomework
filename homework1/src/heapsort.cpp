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
    int left = 2 * i + 1;//�l���`�I
    int right = 2 * i + 2;//�l�k�`�I
    int big = i; //�����]�����`�I

    //�u�n�٦����`�I�N��
    if (right < arraySize && arr[right] > arr[big]) {
        big = right;
    }
    if (left < arraySize && arr[left] > arr[big]) {
        big = left;
    }

    //�p�Gbig�D�̤j
    if (big != i) {
        int temp = arr[i];
        arr[i] = arr[big];
        arr[big] = temp;
        heapify(arr, arraySize, big);//�~��ƧǪ��짹��
    }
}

// �إ̤߳j��G�q�̫�@�ӫD���`�I�}�l�A�v�@�V�U�վ�
void buildMaxHeap(std::vector<int>& arr) {
    int n = static_cast<int>(arr.size());
    for (int i = n / 2 - 1; i >= 0; --i) {
        heapify(arr, n, i);
    }
}

// ��n�ƧǥD�禡
void heapSort(std::vector<int>& arr) {
    int n = static_cast<int>(arr.size());
    buildMaxHeap(arr);//���إ̤߳j��

    for (int heapSize = n; heapSize > 1; --heapSize) {//��̤j��̫�@�ӳ̤p�洫
        int temp = arr[0];
        arr[0] = arr[heapSize - 1];
        arr[heapSize - 1] = temp;

        heapify(arr, heapSize - 1, 0);//���W�Ƨ�
    }
}

int main() {
    int n;
    std::cout << "�п�J�n�h�ּƦr";
    std::cin >> n;

    std::vector<int> data = generateRandomArray(n);

    std::cout << "�Ƨǫe�G";
    for (int v : data) std::cout << v << ' ';
    std::cout << '\n';

    heapSort(data);

    std::cout << "�Ƨǫ�G";
    for (int v : data) std::cout << v << ' ';
    std::cout << '\n';

    return 0;
}
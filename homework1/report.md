# 41243212
# 41243223

# 排序演算法效率分析報告

## 解題說明

### 解題策略



## 程式實作

### Insertion sort (插入排序法)
```c++
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
```
### Quick Sort (快速排序法)
```c++
// Function to find the median of three elements and use it as pivot
int medianOfThree(std::vector<int>& arr, int low, int high) {
    int mid = low + (high - low) / 2;
    if (arr[low] > arr[mid]) std::swap(arr[low], arr[mid]);
    if (arr[low] > arr[high]) std::swap(arr[low], arr[high]);
    if (arr[mid] > arr[high]) std::swap(arr[mid], arr[high]);

    // Place the pivot at the right position by swapping with high
    std::swap(arr[mid], arr[high]);
    return arr[high];
}

// Partition function
int partition(std::vector<int>& arr, int low, int high) {
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
void quickSort(std::vector<int>& arr, int low, int high) {
    if (low < high) {
        int pivotIndex = partition(arr, low, high);
        quickSort(arr, low, pivotIndex - 1);
        quickSort(arr, pivotIndex + 1, high);
    }
}
```

## 效能分析

## 測試與驗證

### 測試案例

####  Worst-case 耗費時間(milliseconds)
| 資料筆數 $n$ | Insertion sort (插入排序法) | Quick sort (快速排序法) | Merge sort (合併排序法) | Heap sort (堆積排序法) |
|--------------|-----------------------------|-------------------------|-------------------------|------------------------|
| $n = 500$    | 0.0331                      | 0.0164                  | 0.04516                 | 0.04774                |
| $n = 1000$   | 0.1243                      | 0.0493                  | 0.04516                 | 0.05304                |
| $n = 2000$   | 0.5109                      | 0.176                   | 0.09788                 | 0.1095                 |
| $n = 3000$   | 1.0059                      | 0.3524                  | 0.156                   | 0.2035                 |
| $n = 4000$   | 1.835                       | 0.5912                  | 0.21262                 | 0.25766                |
| $n = 5000$   | 2.76                        | 1.0207                  | 0.28352                 | 0.31126                |

#### Average-case 耗費時間(milliseconds)
| 資料筆數 $n$ | Insertion sort (插入排序法) | Quick sort (快速排序法) | Merge sort (合併排序法) | Heap sort (堆積排序法) |
|--------------|-----------------------------|-------------------------|-------------------------|------------------------|
| $n = 500$    | 0.0234736                   | 0.0045201               | 0.0071341               | 0.006861               |
| $n = 1000$   | 0.121091                    | 0.01041                 | 0.015679                | 0.0150962              |
| $n = 2000$   | 0.371795                    | 0.0233149               | 0.0600007               | 0.072763               |
| $n = 3000$   | 0.749515                    | 0.0824432               | 0.109856                | 0.135663               |
| $n = 4000$   | 1.27272                     | 0.126867                | 0.163584                | 0.207448               |
| $n = 5000$   | 1.84374                     | 0.169902                | 0.217879                | 0.254465               |

### 編譯與執行指令

```shell
$ g++ -std=c++17 -o sigma sigma.cpp
$ ./sigma
6
```

### 結論



## 申論及開發報告



# 41243212
# 41243223

# 排序演算法效率分析報告

## 解題說明

### 解題策略

- 討論每種排序法的 Worst-case 與 Average-case 時間複雜度：
  - Insertion Sort
  - Quick Sort（使用 median-of-three 選 pivot）
  - Merge Sort（使用 iterative 方法）
  - Heap Sort
  - Composite Sort（結合多種排序法）
 -  對於每個排序法，針對以下輸入大小各別生成符合該演算法的最壞及正常測資：
    - `n = 500, 1000, 2000, 3000, 4000, 5000`
    - 測量每次排序所需的時間與記憶體用量並畫出圖表
    - 比較理論時間複雜度與實際測試結果，確認演算法在理論與實測上是否一致

## 程式實作

### Insertion sort (插入排序法)

把資料分成兩串，一串為排好的，另一串沒有排好，然後開始選取另一串沒排好的數字插入排好的排序中，如果比它大就把排好的排序往右推一格。

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


### Quick sort (快速排序法)

通過medianOfThree策略來選取pivot做為依據，比pivot小的放左邊，比它大的放右邊，同時繼續quicksort被分成兩串的排序直到完成。

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

### Merge sort (合併排序法)

```c++
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
	for (int l = 1; l < n; l *= 2) {
		MergePass(a, temp, n, l);
		l *= 2;
		MergePass(temp, a, n, l);
	}
}
```

### Heap sort (堆積排序法)

```c++
void heapify(std::vector<int>& arr, int arraySize, int i) {
	totalMemoryUsed += 3 * sizeof(int);
	heapifyCallDepth++;
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
		std::swap(arr[i], arr[big]);
        heapify(arr, arraySize, big);//繼續排序直到完成
    }
}

// 建立最大堆：從最後一個非葉節點開始，逐一向下調整
void buildMaxHeap(std::vector<int>& arr) {
    int n = static_cast<int>(arr.size());
	totalMemoryUsed += sizeof(int); // for n

    for (int i = n / 2 - 1; i >= 0; --i) {
        heapify(arr, n, i);
    }
}

// 堆積排序主函式
void heapSort(std::vector<int>& arr) {
    int n = static_cast<int>(arr.size());
	totalMemoryUsed += sizeof(int); // n
    buildMaxHeap(arr);//先建立最大堆

    for (int heapSize = n; heapSize > 1; --heapSize) {//把最大跟最後一個最小交換
		std::swap(arr[0], arr[heapSize - 1]);

        heapify(arr, heapSize - 1, 0);//遞規排序
    }
}
```

### Composite Sort
#### Composite Sort 程式邏輯
```
compositeSort(arr):
  ├── if arr.size() <= 16:
  │     └── insertionSort
  ├── else if alreadyNearlySorted(arr):
  │     └── mergeSort
  └── else:
        └── quickSort(arr, depthLimit = 2 * log2(n))
                ├── if depthLimit == 0:
                │     └── heapSort
                └── if small segment:
                      └── insertionSort

```
#### 整體策略邏輯

| 資料特性 | 採用演算法 | 原因 |
|----------|-------------|------|
| 資料量小（≤16） | Insertion Sort | 開銷極低，快且穩定 |
| 幾乎已排序 (>95%) | Merge Sort | 穩定、預測性高，對近乎排序資料表現佳 |
| 資料量大、亂序 | Quick Sort | 分治法效能佳，平均時間複雜度 $O(n\log n)$ |
| Quick Sort 遞迴太深（退化） | Heap Sort | 最壞情況保證 $O(n\log n)$，避免 quick sort 退化為 $O(n^2)$ |

### 耗時計算

使用 chrono 函式庫來計算呼叫排序算法的耗時。為了避免誤差，實作時會運行5次相同排序，並取平均耗時作為結果。
```c++
auto start = std::chrono::high_resolution_clock::now();
Sort(input);
auto end = std::chrono::high_resolution_clock::now();
std::chrono::duration<double> elapsed = end - start;
time = elapsed.count() * 1000.0; // Convert to milliseconds
```

### Worst-case 測資生成

#### Insertion sort (插入排序法)
生成 [n, n-1, n-2, ... , 1] 的輸入
```c++
std::vector<int> generateInsertionSortWorst(int n) {
	std::vector<int> arr(n);
	for (std::vector<int>::iterator it = arr.begin(); it != arr.end(); it++)
	{
		*it = n - (it - arr.begin());
	}
	return arr;
}
```

#### Quick sort (快速排序法)
在 Quick Sort 中選擇 pivot 時，Median of Three 策略會取：
`pivot = median(arr[start], arr[mid], arr[end])`
createBadInput這個遞迴函式，它將設計過的數字填入陣列，使 Quick Sort 中的 Median of Three 每次都選到最糟的 pivot，因次時間複雜度會退化為 $O(n^2)$。
```c++
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
```
#### Merge sort (合併排序法) 及 Heap sort (堆積排序法)
因為這兩個排序法的平均及最差情況皆為$O(n \log n)$，在測試耗費時間時，使用的測資是使用1000次生成中耗時最久的測資做為結果。
```c++
int generateWorst() {
	int sizes[] = { 500, 1000, 2000, 3000, 4000, 5000 };
	int n; // Size of the array
	int tries = 1000; // 重複生成次數
	int reruns = 5; // 執行5次取平均 確保耗時準確性

	double maxAvgTime = 0;
	std::vector<int> slowestInput;
	std::vector<int> slowestOutput;
	for (int k = 0; k < 6; k++) {
		n = sizes[k];
		// 執行1000次生成及排序，紀錄耗時最久的測資
		for (int i = 0; i < tries; ++i) {
			std::vector<int> input = generateRandomArray(n);
			std::vector<int> copy;
			double totalTime = 0.0;

			for (int j = 0; j < reruns; ++j) {
				copy = input;
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
			}
		}

		std::cout << "n=" << n << "\nWorst execution time : " << maxAvgTime << " ms\n";
	}
}
```

### Average-case 測資生成
使用以下函式生成內含隨機數字的 vector。在計算耗時時，使用以下函式隨機生成 1000 次測資，取每次測試的平均結果。
```c++
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
```

## 效能分析

### 時間複雜度
| 演算法 | Best | Worst | Average |
|--------|------|-------|---------|
| Insertion sort (插入排序法) | $$O(n)$$ | $$O(n^2)$$ | $$O(n^2)$$ |
| Quick sort (快速排序法) | $$O(n \log n)$$ | $$O(n^2)$$ | $$O(n \log n)$$ |
| Merge sort (合併排序法) | $$O(n \log n)$$ | $$O(n \log n)$$ | $$O(n \log n)$$ |
| Heap sort (堆積排序法) | $$O(n \log n)$$ | $$O(n \log n)$$ | $$O(n \log n)$$ |

### 空間複雜度

| 演算法 | 空間複雜度 |
|--------|------------|
| Insertion sort (插入排序法) | $$O(1)$$ |
| Quick sort (快速排序法) | $$O(\log n) \sim O(n)$$ |
| Merge sort (合併排序法) | $$O(n)$$ |
| Heap sort (堆積排序法) | $$O(\log n)$$ |

> [!NOTE]
> Heap sort 使用 Recursion 版本，若為 Iterative 版本則為 $O(1)$ 。

## 測試與驗證

####  Worst-case 耗費時間(milliseconds)
| 資料筆數 $n$ | Insertion sort (插入排序法) | Quick sort (快速排序法) | Merge sort (合併排序法) | Heap sort (堆積排序法) |
|--------------|-----------------------------|-------------------------|-------------------------|------------------------|
| $n = 500$    | 0.0331                      | 0.0164                  | 0.04516                 | 0.04774                |
| $n = 1000$   | 0.1243                      | 0.0493                  | 0.04516                 | 0.05304                |
| $n = 2000$   | 0.5109                      | 0.176                   | 0.09788                 | 0.1095                 |
| $n = 3000$   | 1.0059                      | 0.3524                  | 0.156                   | 0.2035                 |
| $n = 4000$   | 1.835                       | 0.5912                  | 0.21262                 | 0.25766                |
| $n = 5000$   | 2.76                        | 1.0207                  | 0.28352                 | 0.31126                |

![Worst-case Running Time vs Input Size](https://i.imgup.co/zPi1c.png)
#### Average-case 耗費時間(milliseconds)
| 資料筆數 $n$ | Insertion sort (插入排序法) | Quick sort (快速排序法) | Merge sort (合併排序法) | Heap sort (堆積排序法) |
|--------------|-----------------------------|-------------------------|-------------------------|------------------------|
| $n = 500$    | 0.0234736                   | 0.0045201               | 0.0071341               | 0.006861               |
| $n = 1000$   | 0.121091                    | 0.01041                 | 0.015679                | 0.0150962              |
| $n = 2000$   | 0.371795                    | 0.0233149               | 0.0600007               | 0.072763               |
| $n = 3000$   | 0.749515                    | 0.0824432               | 0.109856                | 0.135663               |
| $n = 4000$   | 1.27272                     | 0.126867                | 0.163584                | 0.207448               |
| $n = 5000$   | 1.84374                     | 0.169902                | 0.217879                | 0.254465               |

![Average-case Running Time vs Input Size](https://i.imgup.co/zPPdS.png)
#### Worst-case 記憶體使用量(bytes)
| 資料筆數 $n$ | Insertion sort (插入排序法) | Quick sort (快速排序法) | Merge sort (合併排序法) | Heap sort (堆積排序法) |
|--------------|-----------------------------|-------------------------|-------------------------|------------------------|
| $n = 500$    | 12                          | 2044                    | 2004                    | 116                    |
| $n = 1000$   | 12                          | 3984                    | 4004                    | 128                    |
| $n = 2000$   | 12                          | 7544                    | 8004                    | 140                    |
| $n = 3000$   | 12                          | 11324                   | 12004                   | 152                    |
| $n = 4000$   | 12                          | 14124                   | 16004                   | 152                    |
| $n = 5000$   | 12                          | 17884                   | 20004                   | 164                    |

![Worst-case Memory Usage (bytes)](https://i.imgup.co/zPy0j.png)
#### Composite Sort 與 std::sort 耗費時間(milliseconds)
| 資料筆數 $n$ | Composite Sort | std::sort |
|--------------|----------------|-----------|
| $n = 500$    | 0.0028872      | 0.0044746 |
| $n = 1000$   | 0.0061         | 0.0089749 |
| $n = 2000$   | 0.0135987      | 0.0246504 |
| $n = 3000$   | 0.021979       | 0.0738061 |
| $n = 4000$   | 0.0786211      | 0.10292   |
| $n = 5000$   | 0.127512       | 0.144991  |
| $n = 10000$  | 0.325728       | 0.353554  |
| $n = 100000$ | 4.33169        | 4.64411   |

### 結論



## 申論及開發報告


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

把資料分成兩串，一串為排好的，另一串沒有排好，然後開始選取另一串沒排好的數字插入排好的排序中，如果比它大就把排好的排序往右推一格。

### Quick sort (快速排序法)
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

通過medianOfThree策略來選取pivot做為依據，比pivot小的放左邊，比它大的放右邊，同時繼續quicksort被分成兩串的排序直到完成。

### Merge sort (合併排序法)
```
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
```
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
| Heap sort (堆積排序法) | $$O(1)$$ |

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
| $n = 500$    | 12                          | 2044                    | 2004                    | 53788                  |
| $n = 1000$   | 12                          | 3984                    | 4004                    | 119016                 |
| $n = 2000$   | 12                          | 7544                    | 8004                    | 261616                 |
| $n = 3000$   | 12                          | 11324                   | 12004                   | 414404                 |
| $n = 4000$   | 12                          | 14124                   | 16004                   | 571416                 |
| $n = 5000$   | 12                          | 17884                   | 20004                   | 735388                 |


### 結論



## 申論及開發報告


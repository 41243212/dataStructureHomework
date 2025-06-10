
# 41243212
# 41243223

# Max/Min Heap 與 Binary Search Tree 實作

## 解題說明

### 解題策略

- Max/Min Heap
  - 使用 MinPQ 的 ADT 實現 MinHeap Class
  - 複雜度與原本的 MaxHeap 相同
-  Binary Search Tree
  - 實現二元搜尋樹
  - 使用均匀分布隨機產生 n 個結果，插入二元搜尋樹中
  - 測試 n = 100, 500, 1000, 2000, 3000, ..., 10000 產生出來的樹的高度

## 程式實作

### Max/Min Heap
HeapifyUp 函式會在插入新元素到堆的尾端後，把元素往上調整到正確的位置。
- 對於節點 i，其父節點 (i - 1) / 2 的值要小於或等於它的值。
- 若不符合這個條件，就必須和父節點對調。
```c++
void HeapifyUp(size_t i) {
    while (i > 0 && data[i] < data[(i - 1) / 2]) {
        std::swap(data[i], data[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}
```

HeapifyDown 函式是在刪除堆頂端的最小值後，會用堆的最後一個元素來補上堆頂，這時就要再往下調整堆。
- 比較目前節點與兩個子節點中較小者。
- 若該子節點小於目前節點，就交換並繼續往下。
```c++
void HeapifyDown(size_t i) {
    int size = data.size();
    while (2 * i + 1 < size) {
        int child = 2 * i + 1;
        if (child + 1 < size && data[child + 1] < data[child]) {
            child++;
        }
        if (data[i] <= data[child]) break;
        std::swap(data[i], data[child]);
        i = child;
    }
}
```

IsEmpty 函式，回傳vector是否為空
```c++
bool IsEmpty() const override {
    return data.empty();
}
```

Top 函式，回傳頂端的值
```c++
const T& Top() const override {
    if (IsEmpty()) throw std::runtime_error("Priority queue is empty");
    return data[0];
}
```

Top 函式，插入一筆資料到堆底，並執行 HeapifyUp
```c++
void Push(const T& value) override {
    data.push_back(value);
    HeapifyUp(data.size() - 1);
}
```

Pop 函式，刪除最頂端的最小值，並執行 HeapifyDown
```c++
void Pop() override {
    if (IsEmpty()) throw std::runtime_error("Priority queue is empty");
    data[0] = data.back();
    data.pop_back();
    HeapifyDown(0);
}
```
### Binary Search Tree

節點的結構，包含 pair<K, E> 資料、左子節點指標及右子節點指標。
```c++
template <class K, class E>
struct BSTNode {
    pair<K, E> data;
    BSTNode* left;
    BSTNode* right;

    BSTNode(const pair<K, E>& thePair) : data(thePair), left(nullptr), right(nullptr) {}
};

```

Get 函式，從root開始找，以遞迴呼叫 getHelper 往子節點找，直到找到目標值後回傳。
```c++
pair<K, E>* getHelper(BSTNode<K, E>* node, const K& key) const {
    if (!node) return nullptr;
    if (key < node->data.first)
        return getHelper(node->left, key);
    if (key > node->data.first)
        return getHelper(node->right, key);
    return &node->data;
}
pair<K, E>* Get(const K& key) const override {
    return getHelper(root, key);
}
```
Insert 函式，比較傳入的 pair<K, E> 的值，若小於目前節點的值，則以遞迴的方式往左子節點的方向尋找適合的擺放位置，反之則往右邊找，直到子節點為空值，表示找到插入位置，創建新節點並return回去。
```c++
BSTNode<K, E>* insertHelper(BSTNode<K, E>* node, const pair<K, E>& thePair) {
    if (!node)
        return new BSTNode<K, E>(thePair);
    if (thePair.first < node->data.first)
        node->left = insertHelper(node->left, thePair);
    else if (thePair.first > node->data.first)
        node->right = insertHelper(node->right, thePair);
    else
        node->data.second = thePair.second; // Update existing
    return node;
}
void Insert(const pair<K, E>& thePair) override {
    root = insertHelper(root, thePair);
}
```
Delete 函式，這個函式分為兩個步驟，第一步是尋找節點，根據小或大分別往左或右子節點尋找要刪除的目標節點，第二步是找到節點後的操作。
- 找到節點後的操作
  - 無子節點: 回傳 node->right(此時是nullptr)給父節點，並刪除節點。
  - 有左或右子節點: 回傳左或右子節點給父節點，並刪除節點。
  - 有兩個子節點: 找到右子樹下的最小節點(沿左子樹走到底)，取出作為刪除節點後的後繼者，並將已經取出來的後繼者刪除。
```c++
BSTNode<K, E>* deleteHelper(BSTNode<K, E>* node, const K& key) {
    if (!node) return nullptr;

    if (key < node->data.first)
        node->left = deleteHelper(node->left, key);
    else if (key > node->data.first)
        node->right = deleteHelper(node->right, key);
    else {
        // Node with one or no child
        if (!node->left) {
            BSTNode<K, E>* temp = node->right;
            delete node;
            return temp;
        }
        else if (!node->right) {
            BSTNode<K, E>* temp = node->left;
            delete node;
            return temp;
        }
        // Node with two children: replace with inorder successor
        BSTNode<K, E>* successor = node->right;
        while (successor->left)
            successor = successor->left;
        node->data = successor->data;
        node->right = deleteHelper(node->right, successor->data.first);
    }
    return node;
}
void Delete(const K& key) override {
    root = deleteHelper(root, key);
}
```

解構時會使用到的刪除所有子節點的遞迴程式。
```c++
void destroyTree(BSTNode<K, E>* node) {
    if (!node) return;
    destroyTree(node->left);
    destroyTree(node->right);
    delete node;
}
```

以遞迴方式計算樹的高度。
```c++
int heightHelper(BSTNode<K, E>* node) const {
    if (!node) return -1; // empty tree has height -1
    int leftHeight = heightHelper(node->left);
    int rightHeight = heightHelper(node->right);
    return 1 + max(leftHeight, rightHeight);
}
```

中序走訪，將樹由小輸出到大的，用於檢查程式正常運行。
```c++
void inorderHelper(BSTNode<K, E>* node) {
    if (!node) return;
    inorderHelper(node->left);
    cout << "(" << node->data.first << ", " << node->data.second << ") ";
    inorderHelper(node->right);
}
```
隨機均勻分布生成要插入的值，按照題目要求的 n，逐一生成並計算比值。
```c++
int main() {
    std::random_device rd;  // a seed source for the random number engine
    std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distrib(1, 100000);
    int ns[] = {100, 500, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000};
    for (int n = 0; n < 12; ++n) {
        BSTDictionary<int, int> dict;
        for (int i = 0; i < ns[n]; ++i) {
            int key = distrib(gen);
            dict.Insert(make_pair(key, 0));
        }
        cout << "n = " << ns[n] << endl;
        //dict.InorderTraversal();
        cout << "Height of the tree: " << dict.Height() << endl;
        cout << "Height/log2(n): " << dict.Height() / log2(ns[n]) << endl;
    }

    return 0;
}
```

## 效能分析

### MinHeap
| 操作        | 時間複雜度    | 空間複雜度 |
| --------- | -------- | ----- |
| `Push`    | O(log n) | O(n)  |
| `Pop`     | O(log n) | O(n)  |
| `Top`     | O(1)     | O(n)  |
| `IsEmpty` | O(1)     | O(n)  |

### Binary Search Tree
| 操作                 | 時間複雜度 | 空間複雜度       |
| ------------------ | ----- | ----------- |
| `Get`              | O(h)  | O(h) (遞迴堆疊) |
| `Insert`           | O(h)  | O(h) (遞迴堆疊) |
| `Delete`           | O(h)  | O(h) (遞迴堆疊) |

## 測試與驗證

####  Binary Search Tree 不同高度下與log_2(n)比值
| 資料筆數 $n$ | 高度 | $\frac{\text{height}}{\log_2(n)}$ |
|--------------|------|-----------------------------------|
| $n = 100$    | 12   | 1.80618                           |
| $n = 500$    | 18   | 2.00763                           |
| $n = 1000$   | 20   | 2.00687                           |
| $n = 2000$   | 21   | 1.91505                           |
| $n = 3000$   | 24   | 2.07779                           |
| $n = 4000$   | 24   | 2.00572                           |
| $n = 5000$   | 26   | 2.11593                           |
| $n = 6000$   | 30   | 2.3903                            |
| $n = 7000$   | 28   | 2.1921                            |
| $n = 8000$   | 28   | 2.15953                           |
| $n = 9000$   | 27   | 2.05547                           |
| $n = 10000$  | 28   | 2.10721                           |

### 結論

與課本上所述的結論相同，可以觀察到在任何 n 下，比值都接近為 2。

## 申論及開發報告

本次作業實作了 MinHeap 與 Binary Search Tree 兩種資料結構。MinHeap 使用向上與向下堆化來維持堆的性質，具備良好的時間效率（Push 和 Pop 操作為 O(log n)）。

BST 部分，採用隨機均勻分布插入節點，實測不同資料筆數下的樹高皆維持在 $\log_2(n)$ 的兩倍以內，符合預期結果。從測試結果可得知，即便沒有進行平衡操作，隨機插入的 BST 高度依然會符合課本結論。

整體而言，本次實作驗證了兩種資料結構的時間與空間複雜度，並透過實驗佐證了理論分析的正確性。

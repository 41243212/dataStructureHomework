# Exercise 3 - External Sorting: Input Time and CPU Time Relationship

## 1. Problem Description

有 $n = 200,000$ 筆資料要進行排序，但記憶體大小為 $S$，其中 $S \ll n$，代表無法一次將所有資料載入。假設記憶體可完全作為輸入/輸出緩衝區使用。

磁碟中已經有 $m$ 個 sorted runs，從磁碟讀取資料的成本如下：
- 尋找時間（Seek time）：$t_s = 0.08$ 秒
- 延遲時間（Latency）：$t_l = 0.02$ 秒
- 傳輸一筆資料所需時間：$t_r = 0.001$ 秒

採用 $k$ 路合併排序，並將記憶體劃分為輸入與輸出緩衝區，允許輸入、輸出與 CPU 同時進行。

## 2. Input Time Calculation

### 2.1 Formula

每筆資料的讀取時間為：
$$
t_{\text{record}} = t_s + t_l + t_r
$$

總輸入時間為：
$$
t_{\text{input}} = n \cdot (t_s + t_l + t_r)
$$

### 2.2 Calculation

代入數值：
- $n = 200,000$
- $t_s = 0.08$
- $t_l = 0.02$
- $t_r = 0.001$

計算如下：
$$
t_{\text{input}} = 200,000 \cdot (0.08 + 0.02 + 0.001) = 200,000 \cdot 0.101 = 20,200 \text{ 秒}
$$

### 結論

總輸入時間為 **20,200 秒**。

---

## 3. Relationship Between CPU Time and Input Time

### 3.1 Description

假設合併所有 runs 所需的 CPU 時間為 $t_{\text{CPU}}$，並且這個值可以視為常數。

問題為：是否總存在一個 $k$ 值，使得下列近似成立？

$$
t_{\text{CPU}} \approx t_{\text{input}}
$$

### 3.2 Analysis

- 當 $k$ 增加時，系統能更有效率地處理輸入，$t_{\text{input}}$ 傾向減少。
- 而 $t_{\text{CPU}}$ 為常數，因此只要調整適當的 $k$，即可使兩者時間相近。

### 結論

**是的，總存在某個 $k$ 值，使得 $t_{\text{CPU}} \approx t_{\text{input}}$。**

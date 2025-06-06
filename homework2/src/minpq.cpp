#include <iostream>
#include <vector>

// ADT 5.2
template <class T>
class MaxPQ {
public:
    virtual ~MaxPQ() {}
    virtual bool IsEmpty() const = 0;
    virtual const T& Top() const = 0;
    virtual void Push(const T&) = 0;
    virtual void Pop() = 0;
};

template <class T>
class MinPQ : public MaxPQ<T> {
private:
    std::vector<T> data;

    void HeapifyUp(size_t i) {
        while (i > 0 && data[i] < data[(i - 1) / 2]) {
            std::swap(data[i], data[(i - 1) / 2]);
            i = (i - 1) / 2;
        }
    }

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
public:
    bool IsEmpty() const override {
        return data.empty();
    }

    const T& Top() const override {
        if (IsEmpty()) throw std::runtime_error("Priority queue is empty");
        return data[0];
    }

    void Push(const T& value) override {
        data.push_back(value);
        HeapifyUp(data.size() - 1);
    }

    void Pop() override {
        if (IsEmpty()) throw std::runtime_error("Priority queue is empty");
        data[0] = data.back();
        data.pop_back();
        HeapifyDown(0);
    }

};

int main() {
    MinPQ<int> pq;
    pq.Push(5);
    pq.Push(3);
    pq.Push(8);
    pq.Push(1);

    while (!pq.IsEmpty()) {
        std::cout << pq.Top() << " ";
        pq.Pop();
    }
    std::cout << std::endl;

    return 0;

}
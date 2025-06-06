#include <iostream>
#include <random>
#include <cmath>
#include <utility>
using namespace std;

template <class K, class E>
class Dictionary {
public:
    virtual bool IsEmpty() const = 0;
    virtual pair<K, E>* Get(const K&) const = 0;
    virtual void Insert(const pair<K, E>&) = 0;
    virtual void Delete(const K&) = 0;
    virtual ~Dictionary() {}
};

template <class K, class E>
struct BSTNode {
    pair<K, E> data;
    BSTNode* left;
    BSTNode* right;

    BSTNode(const pair<K, E>& thePair) : data(thePair), left(nullptr), right(nullptr) {}
};

// Binary Search Tree Dictionary
template <class K, class E>
class BSTDictionary : public Dictionary<K, E> {
private:
    BSTNode<K, E>* root;

    // Helper for Get
    pair<K, E>* getHelper(BSTNode<K, E>* node, const K& key) const {
        if (!node) return nullptr;
        if (key < node->data.first)
            return getHelper(node->left, key);
        if (key > node->data.first)
            return getHelper(node->right, key);
        return &node->data;
    }

    // Helper for Insert
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

    // Helper for Delete
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

    void destroyTree(BSTNode<K, E>* node) {
        if (!node) return;
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }

    int heightHelper(BSTNode<K, E>* node) const {
        if (!node) return -1; // empty tree has height -1
        int leftHeight = heightHelper(node->left);
        int rightHeight = heightHelper(node->right);
        return 1 + max(leftHeight, rightHeight);
    }

    void inorderHelper(BSTNode<K, E>* node) {
        if (!node) return;
        inorderHelper(node->left);
        cout << "(" << node->data.first << ", " << node->data.second << ") ";
        inorderHelper(node->right);
    }

public:
    BSTDictionary() : root(nullptr) {}

    ~BSTDictionary() {
        destroyTree(root);
    }

    bool IsEmpty() const override {
        return root == nullptr;
    }

    pair<K, E>* Get(const K& key) const override {
        return getHelper(root, key);
    }

    void Insert(const pair<K, E>& thePair) override {
        root = insertHelper(root, thePair);
    }

    void Delete(const K& key) override {
        root = deleteHelper(root, key);
    }

    int Height() const {
        return heightHelper(root);
    }

    void InorderTraversal() {
        inorderHelper(root);
        cout << endl;
    }
};

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

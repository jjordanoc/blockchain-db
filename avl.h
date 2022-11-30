#ifndef AVLTree_H
#define AVLTree_H

#include <queue>
#include <cmath>
#include <iomanip>
#include "index.h"

using namespace std;

template <typename T, typename Cmp>
class AVLTree : public Index
{
private:
    struct NodeBT {
        T data;
        int height;
        NodeBT* left;
        NodeBT* right;
        NodeBT() : left(nullptr), right(nullptr), height(0) {}
        NodeBT(T value) : data(value), left(nullptr), right(nullptr), height(0) {}

        void killSelf(){
            if(left != nullptr) left->killSelf();
            if(right != nullptr) right->killSelf();
            delete this;
        }
    };
    NodeBT *root;
    int sz = 0;
    Cmp compare;
public:
    AVLTree(Cmp& compare) : root(nullptr),compare(compare) {}
    void insert(T value)
    {
        insert(this->root, value);
    }
    bool find(T value)
    {
        return find(this->root, value);
    }
    T search(T data)
    {
        return search(this->root, data);
    }
    string getPreOrder(){
        return getPreOrder(this->root);
    }

    int height()
    {
        return height(this->root);
    }
    T minValue()
    {
        return minValue(this->root);
    }
    T maxValue()
    {
        return maxValue(this->root);
    }
    bool isBalanced()
    {
        return isBalanced(this->root);
    }
    int size()
    {
        return size(this->root);
    }
    void remove(T value)
    {
        remove(this->root, value);
    }
    vector<T> range_search(T a, T b) {
        vector<T> result;
        _range_search(root, a, b, result);
        return result;
    }

    ~AVLTree(){
        if(this->root != nullptr){
            this->root->killSelf();
        }
    }

private:
    void _range_search(NodeBT *&node, T start, T end, vector<T> &result){

        if (node == nullptr) return;
        bool startEq = !compare(node->data, start) && !compare(start, node->data);
        bool endEq = !compare(node->data, end) && !compare(end, node->data);
        if (compare(node->data, start)) {
            _range_search(node->left, start, end, result);
        }
        if ((compare(node->data, start) || startEq) && (compare(end, node->data) || endEq)) {
            result.push_back(node->data);
        }
        if (compare(end, node->data)) {
            _range_search(node->right, start, end, result);
        }
    }
    void _display_preorder(NodeBT *node, string &result) {
        if (node == nullptr) return;
        result += to_string(node->data) + " ";
        _display_preorder(node->left, result);
        _display_preorder(node->right, result);
    }

    string getPreOrder(NodeBT *node) {
        string result;
        _display_preorder(root, result);
        return result;
    }
    void insert(NodeBT *&node, T data) {
        if (node == nullptr) {
            node = new NodeBT();
            node->data = data;
            node->left = nullptr;
            node->right = nullptr;
            node->height = 0;
            ++sz;
        } else {
            if (compare(node->data, data)) {
                insert(node->left, data);
            } else if (compare(data, node->data)) {
                insert(node->right, data);
            }
        }
        updateHeight(node);
        balance(node);
    }//*
    bool find(NodeBT *node, T data) {
        if (node == nullptr)
            return false;
        else if (compare(node->data, data))
            return find(node->left, data);
        else if (compare(data, node->data))
            return find(node->right, data);
        else
            return true;
    }
    T search(NodeBT *node, T data) {
        if (node == nullptr)
            return T{};
        else if (compare(node->data, data))
            return search(node->left, data);
        else if (compare(data, node->data))
            return search(node->right, data);
        else
            return node->data;
    }
    int height(NodeBT *node) {
        if (node != nullptr) {
            return node->height;
        }
        return -1;
    }
    bool isBalanced(NodeBT *node) {
        if (node == nullptr) return true;
        return abs(balancingFactor(node)) <= 1 && isBalanced(node->left) && isBalanced(node->right);
    }
    T minValue(NodeBT *node) {
        while (node->left != nullptr) {
            node = node->left;
        }
        return node->data;
    }
    T maxValue(NodeBT *node) {
        while (node->right != nullptr) {
            node = node->right;
        }
        return node->data;
    }
    int size(NodeBT *node) {
        return sz;
    }
    void remove(NodeBT *&node, T value) {
        if (node == nullptr)
            return;
        else if (compare(node->data, data))
            remove(node->left, value);
        else if (compare(data, node->data))
            remove(node->right, value);
        else
        {
            if (node->left == nullptr && node->right == nullptr)
            {
                delete node; //free
                node = nullptr;
            }
            else if (node->left == nullptr)
            {
                NodeBT *temp = node;
                node = node->right;
                delete temp;
            }
            else if (node->right == nullptr)
            {
                NodeBT *temp = node;
                node = node->left;
                delete temp;
            }
            else
            {
                T temp = maxValue(node->left);
                node->data = temp;
                remove(node->left, temp);
            }
            sz--;
        }
        updateHeight(node);
        balance(node);
    }//*
private:
    /*add for avl*/    
    int balancingFactor(NodeBT *node) {
        if (node == nullptr) return 0;
        return height(node->left) - height(node->right);
    }
    void updateHeight(NodeBT *node) {
        if (node == nullptr) return;
        node->height = max(height(node->left), height(node->right)) + 1;
    }
    void balance(NodeBT *&node) {
        if (node == nullptr) return;
        if (balancingFactor(node) >= 2) {
            if (balancingFactor(node->left) <= -1) {
                left_rota(node->left);
            }
            right_rota(node);
        }
        else if (balancingFactor(node) <= -2) {
            if (balancingFactor(node->right) >= 1) {
                right_rota(node->right);
            }
            left_rota(node);
        }
    }

    void left_rota(NodeBT *&node){
        auto q = node->right;
        node->right = q->left;
        updateHeight(node);
        updateHeight(q);
        q->left = node;
        node = q;
    }
    void right_rota(NodeBT *&node){
        auto q = node->left;
        node->left = q->right;
        q->right = node;
        updateHeight(node);
        updateHeight(q);
        node = q;
    }
};


#endif

#ifndef HEAP_HEAP_H
#define HEAP_HEAP_H

#include <vector>
#include <iostream>
#include "index.h"
using namespace std;

template<typename T, typename Comparable>
class Heap : public Index {
private:
    vector<T> data;
    Comparable cmp; // assume comparison is >
    int parent(int pos) {
        return (pos - 1) / 2;
    }
    int left(int pos) {
        return pos * 2 + 1;
    }
    int right(int pos) {
        return pos * 2 + 2;
    }
    bool valid_position(int size, int pos) {
        return pos >= 0 && pos < size;
    }
    void heapify_down(vector<T> &elements, int pos) {
        // swap ith parent with the greatest child, then call the function recursively
        if (valid_position(elements.size(),left(pos)) && cmp(elements[left(pos)], elements[pos])) {
            // left child exists, and is greater than parent

            // if there exists a right child, we must check if it's greater than the left child
            if (valid_position(elements.size(), right(pos)) && cmp(elements[right(pos)], elements[left(pos)])) {
                // swap with right
                T tmp = elements[right(pos)];
                elements[right(pos)] = elements[pos];
                elements[pos] = tmp;
                heapify_down(elements, right(pos));
            }
            else {
                // swap with left
                T tmp = elements[left(pos)];
                elements[left(pos)] = elements[pos];
                elements[pos] = tmp;
                heapify_down(elements, left(pos));
            }

        }
        else if (valid_position(elements.size(),right(pos)) && cmp(elements[right(pos)], elements[pos])) {
            // right child exists, and is greater than parent

            // if there exists a left child, we must check if it's greater than the right child
            if (valid_position(elements.size(), left(pos)) && cmp(elements[left(pos)], elements[right(pos)])) {
                // swap with left
                T tmp = elements[left(pos)];
                elements[left(pos)] = elements[pos];
                elements[pos] = tmp;
                heapify_down(elements, left(pos));
            }
            else {
                // swap with right
                T tmp = elements[right(pos)];
                elements[right(pos)] = elements[pos];
                elements[pos] = tmp;
                heapify_down(elements, right(pos));
            }
        }
    }
    void heapify_up(int pos) {
        // swap if smaller than parent
        if (valid_position(data.size(), parent(pos)) && cmp(data[pos], data[parent(pos)])) {
            T tmp = data[parent(pos)];
            data[parent(pos)] = data[pos];
            data[pos] = tmp;
            heapify_up(parent(pos));
        }
    }
public:
    Heap() = default;
    explicit Heap(int size, Comparable &cmp) : cmp(cmp) {
        data.reserve(size);
    }
    void push(T value) {
        // insert element at the end of array
        data.push_back(value);
        // apply heapify_up algorithm starting from the end of array
        heapify_up(data.size() - 1);
    }
    void display() {
        for (const auto &v : data) {
            std::cout << v << ",";
        }
        std::cout << std::endl;
    }
    void pop() {
        // swap root with last element, delete last element and heapify down root
        if (data.empty()) return;
        T tmp = data[data.size() - 1];
        data[data.size() - 1] = data[0];
        data[0] = tmp;
        // delete last element
        data.pop_back();
        heapify_down(data, 0);
    }

    T top() {
        if (data.empty()) return T{};
        return data[0];
    }
};


#endif //HEAP_HEAP_H

#ifndef PROYECTO_SHA256_CIRCULAR_LIST_H
#define PROYECTO_SHA256_CIRCULAR_LIST_H

#include <iostream>
#include "list.h"

template<typename T>
class CircularList : public List<T> {
private:
    Node<T> *head;//sentinel
    int _size;
public:
    CircularList() : List<T>() {
        // initialize sentinel
        head = new Node<T>();
        head->next = head;
        head->prev = head;
        _size = 0;
    }

    T front() override {
        // handle sentinel node
        if (head->next == head) {
            throw std::runtime_error("Empty list.");
        }
        return head->next->data;
    }

    T back() override {
        // handle sentinel node
        if (head->prev == head) {
            throw std::runtime_error("Empty list.");
        }
        return head->prev->data;
    }

    void push_front(T t) override {
        auto *node = new Node<T>();
        node->data = t;
        node->next = head->next;
        node->prev = head;
        head->next->prev = node;
        head->next = node;
        _size++;
    }

    void push_back(T t) override {
        auto *node = new Node<T>();
        node->data = t;
        node->next = head;
        node->prev = head->prev;
        head->prev->next = node;
        head->prev = node;
        _size++;
    }

    T pop_front() override {
        // handle sentinel node
        if (head->next == head) {
            throw std::runtime_error("Empty list.");
        }
        Node<T> *tmp = head->next;
        head->next->next->prev = head;
        head->next = head->next->next;
        T tmp_value = tmp->data;
        delete tmp;
        _size--;
        return tmp_value;
    }

    T pop_back() override {
        // handle sentinel node
        if (head->prev == head) {
            throw std::runtime_error("Empty list.");
        }
        Node<T> *tmp = head->prev;
        head->prev->prev->next = head;
        head->prev = head->prev->prev;
        T tmp_value = tmp->data;
        delete tmp;
        _size--;
        return tmp_value;
    }

    T insert(T t, int i) override {
        // handle exceptional cases
        if (i < 0 || i >= _size) {
            throw std::runtime_error("Index out of bounds.");
        }
        Node<T> *tmp = head->next;
        for (int c = 0; c < i; c++) {
            tmp = tmp->next;
        }
        auto *node = new Node<T>();
        node->data = t;
        node->next = tmp;
        node->prev = tmp->prev;
        tmp->prev->next = node;
        tmp->prev = node;
        return t;
    }

    bool remove(int i) override {
        // handle exceptional cases
        if (i < 0 || i >= _size) {
            return false;
        }
        Node<T> *tmp = head->next;
        for (int c = 0; c < i; c++) {
            tmp = tmp->next;
        }
        tmp->prev->next = tmp->next;
        tmp->next->prev = tmp->prev;
        delete tmp;
        return true;
    }

    T &operator[](int i) override {
        // handle exceptional cases
        if (i < 0 || i >= _size) {
            throw std::runtime_error("Index out of bounds.");
        }
        // iterate over list and find ith position
        Node<T> *tmp = head->next;
        for (int c = 0; c < i; c++) {
            tmp = tmp->next;
        }
        return tmp->data;
    }

    bool is_empty() override {
        return _size == 0;
    }

    int size() override {
        return _size;
    }

    void clear() override {
        // delete every node except the sentinel
        Node<T> *tmp = head->next;
        while (tmp != head) {
            tmp = tmp->next;
            delete tmp->prev;
        }
        head->next = head;
        head->prev = head;
        _size = 0;
    }

    void sort() override {
        // insertion sort
        Node<T> *tmp = head->next->next;
        while (tmp != head) {
            Node<T> *tmp2 = tmp;
            // swap values until correct position is reached
            while (tmp2 != head->next && tmp2->prev->data > tmp2->data) {
                // swap
                T tmp_value = tmp2->prev->data;
                tmp2->prev->data = tmp2->data;
                tmp2->data = tmp_value;
                // move back
                tmp2 = tmp2->prev;
            }
            // advance
            tmp = tmp->next;
        }
    }

    bool is_sorted() override {
        // compare each pair of elements
        Node<T> *tmp = head->next;
        for (int i = 0; i < _size; ++i) {
            tmp = tmp->next;
            if (tmp->prev->data > tmp->data) {
                return false;
            }
        }
        return true;
    }

    void reverse() override {
        Node<T> *tmp = head->next;
        // swap prev with next
        for (int i = 0; i <= _size; ++i) {
            // swap pointers
            Node<T> *prev_tmp = tmp->prev;
            tmp->prev = tmp->next;
            tmp->next = prev_tmp;
            tmp = tmp->next;
        }
    }

    ~CircularList() {
        clear();
        delete head;
    }
};

#endif //PROYECTO_SHA256_CIRCULAR_LIST_H

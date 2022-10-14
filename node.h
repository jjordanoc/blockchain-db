#ifndef PROYECTO_SHA256_NODE_H
#define PROYECTO_SHA256_NODE_H


template<typename T>
struct Node {
    T data;
    Node<T> *next;
    Node<T> *prev;

    Node() : next(nullptr), prev(nullptr) {
    }

    explicit Node(T value) : data(value), next(nullptr), prev(nullptr) {
    }

    ~Node() {
        delete data;
    };
};


#endif//PROYECTO_SHA256_NODE_H

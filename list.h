#ifndef PROYECTO_SHA256_LIST_H
#define PROYECTO_SHA256_LIST_H

#include "node.h"

template<typename T>
class List {
protected:
    Node<T> *head;
    Node<T> *tail;

public:
    List() : head(nullptr), tail(nullptr){};

    virtual ~List() = default;

    //Contrato
    virtual T front() = 0;// primer elemento del contenedor
    virtual T back() = 0; // ultimo elemento del contenedor
    virtual void push_front(T) = 0;

    virtual void push_back(T) = 0;

    virtual T pop_front() = 0;

    virtual T pop_back() = 0;

    virtual T insert(T, int) = 0;

    virtual bool remove(int) = 0;

    virtual T &operator[](int) = 0;// debe ser declarado en cada clase hija O(n)
    virtual bool is_empty() = 0;

    virtual int size() = 0;

    virtual void clear() = 0;

    //    virtual void sort() = 0; // comparables
    //    virtual bool is_sorted() = 0;

    virtual void reverse() = 0;// revierte fisicamente in-place
};


#endif//PROYECTO_SHA256_LIST_H

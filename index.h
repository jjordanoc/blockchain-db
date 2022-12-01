#ifndef INDEX_H
#define INDEX_H
#include <string>
#include "entry.h"
#include <forward_list>
using namespace std;


class Index{
public:
    string type;
};

template <typename T>
struct IndexT {
    T key;
    forward_list<Entry*>* values;
    void insert(Entry* dt){
        if(values == nullptr) values = new forward_list<Entry*>();
        values->push_front(dt);
    }
    IndexT() = default;
    IndexT(const T& key):key(key){};
    IndexT(const T& key, Entry* entry):key(key){
        values = new forward_list<Entry*>();
        values->push_front(entry);
    };
    IndexT(const T& key, forward_list<Entry*>* value): key(key), values(values){};
    ~IndexT(){}
};

#endif // INDEX_H

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
    IndexT(){
        values = new forward_list<Entry*>();
    };
    IndexT(const IndexT& other){
        key = other.key;
        values = other.values;
    }
    IndexT(const T& key):key(key){
        values = new forward_list<Entry*>();
    };
    IndexT(const T& key, Entry* entry):key(key){
        values = new forward_list<Entry*>();
        values->push_front(entry);
    };
    IndexT(const T& key, forward_list<Entry*>* value): key(key), values(values){};
    ~IndexT(){}
};

#endif // INDEX_H

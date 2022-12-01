
#ifndef CHAINHASH_CHAINHASH_H
#define CHAINHASH_CHAINHASH_H

#include <forward_list>
#include <unordered_map>
#include "index.h"

using namespace std;

const float maxFillFactor = 0.5;
const int maxColission = 3;

template<typename TK, typename TV>
class ChainHash;


template<typename TK, typename TV>
class ChainHash : public Index {
public:
    struct EntryH {
        TK key;
        TV value;
        size_t hashcode;
        EntryH() = default;
        EntryH(TK key, TV value, size_t hashcode) : key(key), value(value), hashcode(hashcode) {}
        bool operator==(const ChainHash<TK, TV> &other) {
            return key == other.key;
        }
    };
private:
    forward_list<EntryH> *buckets = nullptr;
    int capacity = 0; // tamano real del hash table
    int size = 0; // cantidad de entradas
    hash<TK> getHash{};

public:

    ChainHash(int capacity=7) : capacity(capacity) {
        this->buckets = new forward_list<EntryH>[this->capacity];
        this->size = 0;
    }

    float fillFactor() {
        return (this->size) / (this->capacity * maxColission);
    }

    void insert(TK key, TV value) {
        // si el factor de llenado es mayor al factor de llenado maximo
        if (fillFactor() >= maxFillFactor) rehashing();
        size_t hashcode = getHash(key);
        int index = hashcode % capacity;
        for (auto itr = this->begin(index); itr != this->end(index); ++itr) {
            // buscar llave
            if (itr->key == key) {
                // se encontro, actualizar valor
                itr->value = value;
                return;
            }
        }
        // no se encontro, insertar valor
        this->buckets[index].push_front(EntryH(key, value, hashcode));
        size++;
        // si el numero de colisiones maximo ha sido superado, hacer rehashing
        if (distance(this->begin(index), this->end(index)) > maxColission) rehashing();
    }

    void rehashing() {
        // crear nuevo arreglo de buckets
        int newCapacity = 2 * this->capacity;
        auto *tmp = new forward_list<EntryH>[newCapacity];
        // insertar cada elemento en su respectiva posicion
        for (int i = 0; i < this->bucket_count(); ++i) {
            for (auto itr = this->begin(i); itr != this->end(i); ++itr) {
                int index = itr->hashcode % newCapacity;
                tmp[index].push_front(*itr);
            }
        }
        // liberar el antiguo arreglo de buckets
        delete[] this->buckets;
        // actualizar el antiguo arreglo con el nuevo
        this->buckets = tmp;
        this->capacity = newCapacity;
    }

    TV get(TK key) {
        // TODO
        // acceder al indice
        size_t hashcode = getHash(key);
        int index = hashcode % capacity;
        // buscar llave
        for (auto itr = this->begin(index); itr != this->end(index); ++itr) {
            if (itr->key == key) {
                // se encontro llave
                return itr->value;
            }
        }
        throw runtime_error("Llave no encontrada.");
    }

    TV& getRef(TK key) {
        // TODO
        // acceder al indice
        size_t hashcode = getHash(key);
        int index = hashcode % capacity;
        // buscar llave
        for (auto itr = this->begin(index); itr != this->end(index); ++itr) {
            if (itr->key == key) {
                // se encontro llave
                return itr->value;
            }
        }
        throw runtime_error("Llave no encontrada.");
    }

    bool find(TK key) {
            // acceder al indice
            size_t hashcode = getHash(key);
            int index = hashcode % capacity;
            // buscar llave
            for (auto itr = this->begin(index); itr != this->end(index); ++itr) {
                if (itr->key == key) {
                    // se encontro llave
                    return true;
                }
            }
            return false;
        }

    void remove(TK key) {
        // TODO
        // acceder al indice
        size_t hashcode = getHash(key);
        int index = hashcode % capacity;
        auto currentListSize = distance(this->begin(index), this->end(index));
        if (currentListSize > 1) {
            // buscar llave
            auto itr = this->begin(index);
            for (; next(itr) != this->end(index); ++itr) {
                if (next(itr)->key == key) {
                    // se encontro llave,
                    break;
                }
            }
            // remover
            this->buckets[index].erase_after(itr);
            size--;
        }
        else if (currentListSize > 0) {
            this->buckets[index].clear();
            size--;
        }
    }

    int bucket_count() {
        return this->capacity;
    }

    typename forward_list<EntryH>::iterator begin(int index) {
        return this->buckets[index].begin();
    }

    typename forward_list<EntryH>::iterator end(int index) {
        return this->buckets[index].end();
    }



    ~ChainHash() {
        delete[] this->buckets;
    }
};

#endif //CHAINHASH_CHAINHASH_H

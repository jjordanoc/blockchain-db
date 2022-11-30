#ifndef PROYECTO_SHA256_BLOCK_H
#define PROYECTO_SHA256_BLOCK_H

#include "crypto.h"
#include "entry.h"
#include "string"

class BlockWidget;

template<size_t BLOCK_SIZE>
class BlockIterator{
    Entry **datos;
    int current = 0;
public:
    BlockIterator(Entry **_datos, int _current):datos(_datos),current(_current){}
    BlockIterator(BlockIterator<BLOCK_SIZE> &bi){
        datos = bi.datos;
        current = bi.current;
    }
    void operator++(){
        ++current;
    }
    void operator--(){
        --current;
    }
    bool operator==(const BlockIterator<BLOCK_SIZE> &bi){
        return current == bi.current;
    }
    bool operator!=(const BlockIterator<BLOCK_SIZE> &bi){
        return current != bi.current;
    }
    Entry* operator*(){
        return datos[current];
    }
    BlockIterator operator=(const BlockIterator &other) {
        current = other.current;
        return *this;
    }
};

template<size_t BLOCK_SIZE>
class Block {
    size_t id = 0;
    size_t nonce = 0;

    Entry *datos[BLOCK_SIZE]{};
    size_t fillCount = 0;

    string *prev = nullptr;
    string *hashCode = nullptr;
    friend class BlockWidget;
    typedef BlockIterator<BLOCK_SIZE> iterator;
public:
    Block() {
        // bloque genesis
        fillCount = BLOCK_SIZE;
        prev = new string(64, '0');
        hashCode = new string();
        mine();
    };

    explicit Block(size_t _id, string *_prev) : id(_id), prev(_prev) {
        hashCode = new string();
        rehash();
    };

    iterator begin(){
        return iterator(datos, 0);
    }

    iterator end(){
        return iterator(datos, fillCount-1);
    }

    Entry **getEntries(){
        return datos;
    }

    size_t getSize(){
        return fillCount;
    }

    void mine() {
        // require the hash to have at least 4 leading zeros
        nonce = 0;

        while (!isValid()) {
            ++nonce;
            rehash();
        }
    }

    string stringify() {
        string str;
        if (datos[0] != nullptr) {
            for (int i = 0; i < fillCount; i++) {
                str += datos[i]->stringify() + " | ";
            }
        }
        str += to_string(nonce) + ",";
        str += to_string(id) + ",";
        str += *prev;
        return str;
    }

    size_t getFillCount() {
        return fillCount;
    }

    void rehash() {
        *hashCode = sha256(stringify());
    }

    bool insertEntry(Entry *transaccion) {
        if (fillCount >= BLOCK_SIZE) { return false; }// block is full
        datos[fillCount++] = transaccion;
        rehash();
        return true;
    }

    void updateEntry(const size_t &entryId, Entry *entry) {
        datos[entryId] = entry;
        rehash();
    }

    Entry *getEntry(const size_t &entryId) {
        rehash();
        return datos[entryId];
    }

    bool isValid() {
        rehash();
        return !hashCode->empty() && hashCode->substr(0, 4) == "0000";
    }

    string print() {
        rehash();
        string s;
        s += to_string(id) + ":\n";
        s += "\tnonce: " + to_string(nonce) + "\n\n";
        if (datos[0] != nullptr) {
            for (int i = 0; i < fillCount; i++) {
                s += "\t" + datos[i]->print() + "\n";
            }
        }

        s += "prev: " + *prev + "\n";
        s += "hash: " + *hashCode + "\n";
        return s;
    }

    string getId() {
        rehash();
        return to_string(id);
    }

    string getNonce() {
        rehash();
        return to_string(nonce);
    }

    string *getPrev() {
        rehash();
        return prev;
    }

    string getData() {
        rehash();
        string s;
        if (datos[0] != nullptr) {
            for (int i = 0; i < fillCount; i++) {
                s += "\t" + datos[i]->print() + "\n";
            }
        }
        return s;
    }

    string *getHashCode() {
        rehash();
        return hashCode;
    }

    ~Block() {
        delete hashCode;
        for (int i = 0; i < fillCount; ++i) {
            delete datos[i];
        }
    }
};


#endif//PROYECTO_SHA256_BLOCK_H

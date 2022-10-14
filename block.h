#ifndef PROYECTO_SHA256_BLOCK_H
#define PROYECTO_SHA256_BLOCK_H

#include "crypto.h"
#include "entry.h"
#include "string"

template<size_t BLOCK_SIZE>
class Block {
    size_t id = 0;
    size_t nonce = 0;

    Entry *datos[BLOCK_SIZE]{};
    size_t fillCount = 0;

    string *prev = nullptr;
    string *hashCode = nullptr;

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

    // Calcula el nonce para que el hash cumpla con el requisito de tener 4 ceros por delante
    void mine() {
        nonce = 0;

        while (!isValid()) {
            // rehashear
            rehash();
            ++nonce;
        }
        cout << *hashCode << endl;
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

    void rehash() {
       *hashCode = sha256(stringify());
    }

    bool insertEntry(Entry *transaccion) {
        if (fillCount >= BLOCK_SIZE) { return false; } // block is full
        datos[fillCount++] = transaccion;
        rehash();
        return true;
    }

    void updateEntry(const size_t &entryId, Entry *entry) {
        datos[entryId] = entry;
        rehash();
    }

    bool isValid() {
        return !hashCode->empty() && hashCode->substr(0, 4) == "0000";
    }

    string print() {
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

    string *getHashCode() { rehash(); return hashCode; }

    string *getPrev() {
        return prev;
    }

    ~Block() {
        delete hashCode;
        for (int i = 0; i < fillCount; ++i) {
            delete datos[i];
        }
    }
};


#endif//PROYECTO_SHA256_BLOCK_H

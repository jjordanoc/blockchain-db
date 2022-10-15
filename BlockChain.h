//
// Created by renato on 10/5/22.
//

#ifndef PROYECTO_SHA256_BLOCKCHAIN_H
#define PROYECTO_SHA256_BLOCKCHAIN_H

#include "block.h"
#include "circular_list.h"

template<size_t BLOCK_SIZE>
class BlockChain : public CircularList<Block<BLOCK_SIZE> *> {
public:
    BlockChain() : CircularList<Block<BLOCK_SIZE> *>() {
        this->head->data = new Block<BLOCK_SIZE>();
    }

    void insertEntry(Entry *entry) {
        // Si no se pudo insertar una entry en el último bloque, quiere decir que está lleno por lo que creamos un nuevo bloque
        if (!this->head->prev->data->insertEntry(entry)) {
            this->push_back(new Block<BLOCK_SIZE>(this->size() + 1, this->head->prev->data->getHashCode()));
            this->head->prev->data->insertEntry(entry);
        }
    }

    void hackEntry(int blockId, int entryId, Entry *entry) {
        if (entryId <= 0 || entryId > BLOCK_SIZE || blockId <= 0 || blockId > this->size()) throw runtime_error("Invalid arguments");
        Block<BLOCK_SIZE> *block = (*this)[blockId - 1];
        block->updateEntry(entryId - 1, entry);
    }

    bool isValid() {
        auto tmp = this->head->next;
        while (tmp != this->head) {
            if (!tmp->data->isValid() || tmp->data->getPrev() != tmp->prev->data->getHashCode()) return false;
            tmp = tmp->next;
        }
        return true;
    }

    void validate() {
        auto tmp = this->head->next;
        while (tmp != this->head) {
            if (!tmp->data->isValid()) {
                tmp->data->mine();
            }
            tmp = tmp->next;
        }
    }

    friend ostream &operator<<(ostream &os, BlockChain<BLOCK_SIZE> &other) {
        auto tmp = other.head->next;
        while (tmp != other.head) {
            os << tmp->data->print() << endl;
            tmp = tmp->next;
        }
        return os;
    }
};


#endif//PROYECTO_SHA256_BLOCKCHAIN_H

//
// Created by renato on 10/5/22.
//

#ifndef PROYECTO_SHA256_BLOCKCHAIN_H
#define PROYECTO_SHA256_BLOCKCHAIN_H

#include "block.h"
#include "circular_list.h"

class MainWindow;

template<size_t BLOCK_SIZE>
class BlockChainIterator {
    typedef Node<Block<BLOCK_SIZE> *> Node;
    Node *current;
public:
    BlockChainIterator(Node *_current) : current(_current) {

    }
    BlockChainIterator(BlockChainIterator &other) {
        current = other.current;
    }
    void operator++() {
        current = current->next;
    }
    void operator--() {
        current = current->prev;
    }
    bool operator==(const BlockChainIterator &other) {
        return current == other.current;
    }
    bool operator!=(const BlockChainIterator &other) {
        return current != other.current;
    }
    Block<BLOCK_SIZE> *operator->() {
        return &(current->data);
    }
    Block<BLOCK_SIZE> *operator*() {
        return current->data;
    }
    BlockChainIterator operator=(const BlockChainIterator &other) {
        current = other.current;
        return *this;
    }
};

template<size_t BLOCK_SIZE>
class BlockChain : protected CircularList<Block<BLOCK_SIZE> *> {
    friend class MainWindow;
    typedef BlockChainIterator<BLOCK_SIZE> iterator;
public:
    BlockChain() : CircularList<Block<BLOCK_SIZE> *>() {
        this->head->data = new Block<BLOCK_SIZE>();
    }

    iterator begin() {
        return iterator{this->head->next};
    }

    iterator end() {
        return iterator{this->head};
    }

    Block<BLOCK_SIZE> *insertEntry(Entry *entry) {
        if (!this->head->prev->data->insertEntry(entry)) {
            this->push_back(new Block<BLOCK_SIZE>(this->size() + 1, this->head->prev->data->getHashCode()));
            this->head->prev->data->insertEntry(entry);
        }
        return this->head->prev->data;
    }

    Block<BLOCK_SIZE> *hackEntry(int blockId, int entryId, Entry *entry) {
        if (entryId <= 0 || entryId > BLOCK_SIZE || blockId <= 0 || blockId > this->size()) throw runtime_error("Invalid arguments");
        Block<BLOCK_SIZE> *block = (*this)[blockId - 1];
        block->updateEntry(entryId - 1, entry);
        return block;
    }

    Entry *searchEntry(int blockId, int entryId) {// search for read only entry
        if (entryId <= 0 || entryId > BLOCK_SIZE || blockId <= 0 || blockId > this->size()) throw runtime_error("Invalid arguments");
        return ((*this)[blockId - 1])->getEntry(entryId-1);
    }

    bool isValid() {
        auto tmp = this->head->next;
        while (tmp != this->head) {
            if (!tmp->data->isValid()) return false;
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

    int size() override {
        return this->_size;
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

//
// Created by renato on 10/5/22.
//

#ifndef PROYECTO_SHA256_BLOCKCHAIN_H
#define PROYECTO_SHA256_BLOCKCHAIN_H

#include "circular_list.h"
#include "block.h"

template<size_t BLOCK_SIZE>
class BlockChain : public CircularList<Block<BLOCK_SIZE>>{
public:
    BlockChain():CircularList<Block<BLOCK_SIZE>>(){}

    void push(const string &_emisor, const string &_receptor, const double &_monto, const unsigned long long &_timestamp){
        Entry transaccion(_emisor, _receptor, _monto, _timestamp);
        // Si no se pudo insertar una transaccion en el último bloque, quiere decir que está lleno por lo que creamos un nuevo bloque
        if(!this->head->prev->data.push(transaccion)){
            this->push_back(Block<BLOCK_SIZE>(this->size()+1, this->head->prev->data.getHashCode()));
            this->head->prev->data.push(transaccion);
        }
    }

    friend ostream& operator<<(ostream& os, BlockChain<BLOCK_SIZE>& other){
        auto tmp = other.head->next;
        int count = 1;
        while(tmp != other.head){
            os << to_string(count++) << ":";
            os << tmp->data.print() << endl;
            tmp = tmp->next;
        }
        return os;
    }
};

























#endif //PROYECTO_SHA256_BLOCKCHAIN_H

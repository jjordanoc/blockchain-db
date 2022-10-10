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
        while(tmp != other.head){
            os << tmp->data.print() << endl;
            tmp = tmp->next;
        }
        return os;
    }

    void hack(int blockId, int entryId, Entry entry){
        if(entryId <= 0 || entryId > BLOCK_SIZE || blockId <= 0 || blockId > this->size()) throw runtime_error("Invalid arguments");

        Block<BLOCK_SIZE>& block = (*this)[blockId - 1];

        // Modificamos un entry de un respectivo bloque
        block[entryId - 1] = entry;
        cout << "Hack successed" << endl;

        // En caso se modifique el último bloque de la lista
        if(blockId == this->size()){
            block.minar();
        } else {
            auto tmp = this->head->next;
            while(tmp != this->head){
                tmp->data.minar();
                auto hash = tmp->data.getHashCode();
                tmp = tmp->next;
                if (tmp != this->head) tmp->data.setPrev(hash); // Validar si se está en el nodo centinela, para no modificar su prev
            }
        }
    }
};

























#endif //PROYECTO_SHA256_BLOCKCHAIN_H

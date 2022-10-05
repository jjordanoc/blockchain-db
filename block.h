#ifndef PROYECTO_SHA256_BLOCK_H
#define PROYECTO_SHA256_BLOCK_H

#include "entry.h"

template<size_t BLOCK_SIZE>
class Block {
    size_t id;
    size_t nonce;

    Entry datos[BLOCK_SIZE];
    size_t fillCount;

    string prev;
    string hashCode;
public:

};


#endif //PROYECTO_SHA256_BLOCK_H

#ifndef PROYECTO_SHA256_BLOCK_H
#define PROYECTO_SHA256_BLOCK_H

#include "string"
#include "entry.h"
#include "crypto.h"

template<size_t BLOCK_SIZE>
class Block {
    size_t id = 0;
    size_t nonce = 0;

    Entry datos[BLOCK_SIZE];
    size_t fillCount = 0;

    string prev;
    string hashCode;
public:
    Block(){
        fillCount = BLOCK_SIZE;
        prev = string(64, '0');

        // Calcular el nonce
        minar();
    };

    explicit Block(size_t _id, string _prev):id(_id),prev(_prev){
        // Calcular el nonce
        minar();
    };

    // Calcula el nonce para que el hash cumpla con el requisito de tener 4 ceros por delante
    void minar(){
        nonce = 0;
        string fourcharacters;

        while(fourcharacters != "0000"){
            // rehashear
            hashCode = sha256(stringify());
            // Se toman los 4 primero caracteres
            fourcharacters = hashCode.substr(0, 4);
            ++nonce;
        }
    }

    string stringify(){
        string str;
        for (int i = 0; i < fillCount; i++){
            str += datos[i].stringify() + " | ";
        }
        str += to_string(nonce) + ",";
        str += to_string(id) + ",";
        str += prev;
        return str;
    }

    size_t getfillCount(){return fillCount;}

    bool push(Entry& transaccion){
        if(fillCount >= BLOCK_SIZE){return false;}
        datos[fillCount++] = transaccion;
        minar();
        return true;
    }

    string print(){
        string s;
        s += to_string(id) + ":\n";
        s += "\tnonce: " + to_string(nonce) + "\n\n";
        for(int i = 0; i < fillCount; i++){
            s += "\t" + datos[i].print() + "\n";
        }
        s += "prev: " + prev + "\n";
        s += "hash: " + hashCode + "\n";
        return s;
    }

    string getPrev(){return prev;}

    string getHashCode(){return hashCode;}

    Entry& operator[](int id){
        return datos[id];
    }

    void setPrev(const string& newhash){
        prev = newhash;
    }

};


#endif //PROYECTO_SHA256_BLOCK_H

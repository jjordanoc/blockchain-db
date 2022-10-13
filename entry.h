#ifndef PROYECTO_SHA256_ENTRY_H
#define PROYECTO_SHA256_ENTRY_H

#include <chrono>
#include <iostream>
#include <time.h>

using namespace std;

struct Entry {
    string emisor;
    string receptor;
    double monto;
    unsigned long long timestamp;

    Entry() = default;
    explicit Entry(const string &_emisor, const string &_receptor, const double &_monto, const unsigned long long &_timestamp) : emisor(_emisor), receptor(_receptor), monto(_monto), timestamp(_timestamp) {
    }
    string getDate() const {
        time_t t = timestamp / 1000;
        return ctime(&t);
    }

    Entry &operator=(const Entry &other) {
        emisor = other.emisor;
        receptor = other.receptor;
        monto = other.monto;
        timestamp = other.timestamp;
        return *this;
    }

    [[nodiscard]] string stringify() const {
        string str;
        str += emisor + ",";
        str += receptor + ",";
        str += to_string(monto) + ",";
        str += to_string(timestamp);
        return str;
    }

    [[nodiscard]] string print() const {
        return emisor + "->" + receptor + " (" + to_string(monto) + "$) " + getDate();
    }
};


#endif//PROYECTO_SHA256_ENTRY_H

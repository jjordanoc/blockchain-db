#ifndef PROYECTO_SHA256_ENTRY_H
#define PROYECTO_SHA256_ENTRY_H

#include <iostream>
#include <chrono>
#include <time.h>

using namespace std;

struct Entry {
    string emisor;
    string receptor;
    double monto;
    unsigned long long timestamp;
    explicit Entry(const string &_emisor, const string &_receptor, const double &_monto, const unsigned long long &_timestamp) : emisor(_emisor), receptor(_receptor), monto(_monto), timestamp(_timestamp) {
    }
    string getDate() const {
        time_t t = timestamp / 1000;
        return ctime(&t);
    }

};


#endif //PROYECTO_SHA256_ENTRY_H

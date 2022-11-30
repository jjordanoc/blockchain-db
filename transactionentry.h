#ifndef SHA256_TRANSACTIONENTRY_H
#define SHA256_TRANSACTIONENTRY_H


#include "entry.h"
#include <chrono>
#include <ctime>
#include <iostream>
#include <utility>

using namespace std;

class EntryWidget;

struct TransactionEntry : public Entry {
    string emisor;
    string receptor;
    double monto{};
    time_t timestamp{};
    friend class EntryWidget;
public:
    TransactionEntry() = default;
    explicit TransactionEntry(string _emisor, string _receptor, const double &_monto, const time_t &_timestamp) : emisor(std::move(_emisor)), receptor(std::move(_receptor)), monto(_monto), timestamp(_timestamp) {
    }

    [[nodiscard]] string getDate() const {
        time_t t = timestamp;
        return ctime(&t);
    }

    TransactionEntry &operator=(const TransactionEntry &other) {
        emisor = other.emisor;
        receptor = other.receptor;
        monto = other.monto;
        timestamp = other.timestamp;
        return *this;
    }

    [[nodiscard]] string stringify() const override {
        string str;
        str += emisor + ",";
        str += receptor + ",";
        str += to_string(monto) + ",";
        str += to_string(timestamp);
        return str;
    }

    [[nodiscard]] string print() const override {
        return emisor + "->" + receptor + " (" + to_string(monto) + "$) " + getDate();
    }
};

#endif//SHA256_TRANSACTIONENTRY_H

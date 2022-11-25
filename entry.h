#ifndef PROYECTO_SHA256_ENTRY_H
#define PROYECTO_SHA256_ENTRY_H

#include <chrono>
#include <ctime>
#include <iostream>

using namespace std;

class EntryWidget;

class Entry {
    friend class EntryWidget;
public:
    Entry() = default;

    [[nodiscard]] virtual string stringify() const = 0;

    [[nodiscard]] virtual string print() const = 0;
};


#endif//PROYECTO_SHA256_ENTRY_H

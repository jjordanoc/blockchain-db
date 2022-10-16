#ifndef SHA256_STUDENTENTRY_H
#define SHA256_STUDENTENTRY_H


#include "entry.h"
#include <chrono>
#include <ctime>
#include <iostream>
#include <utility>

using namespace std;

class StudentEntry : public Entry {
private:
    string nombre;
    double promedio;

public:
    StudentEntry() = default;
    StudentEntry(string nombre, double promedio) : nombre(std::move(nombre)), promedio(promedio) {}

    StudentEntry &operator=(const StudentEntry &other) {
        nombre = other.nombre;
        promedio = other.promedio;
        return *this;
    }

    [[nodiscard]] string stringify() const override {
        string str;
        str += nombre + ",";
        str += to_string(promedio);
        return str;
    }
    [[nodiscard]] string print() const override {
        return nombre + ": " + to_string(promedio);
    }
};


#endif//SHA256_STUDENTENTRY_H

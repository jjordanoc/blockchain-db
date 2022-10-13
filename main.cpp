//
// Created by renato on 10/5/22.
//
#include "BlockChain.h"
#include "fstream"
#include <iostream>
using namespace std;

void read_csv();

template<typename Function, typename... Params>
void time_function(Function &fun, const string &function_name, Params &...params);

int main() {
    BlockChain<5> bc;

    auto test_insert_entry = [&]() {
        bc.push("Renato", "Joaquin", 777, 10020);
        bc.push("Chachi", "Joaquin", 123, 100420);
        bc.push("Renato", "Joaquin", 3, 10020);
        bc.push("Renato", "Joaquin", 5, 10020);
        bc.push("Renato", "Joaquin", 1, 10020);
        bc.push("Renato", "Joaquin", 521, 10020);
        bc.push("Renato", "Joaquin", 14, 10020);
        bc.push("Renato", "Joaquin", 13, 10020);
        bc.push("Renato", "Joaquin", 13, 10020);
        bc.push("Renato", "Joaquin", 13, 10020);

        bc.push("Renato", "Joaquin", 777, 10020);
        bc.push("Chachi", "Joaquin", 123, 100420);
        bc.push("Renato", "Joaquin", 3, 10020);
        bc.push("Renato", "Joaquin", 5, 10020);
        bc.push("Renato", "Joaquin", 1, 10020);
        bc.push("Renato", "Joaquin", 521, 10020);
        bc.push("Renato", "Joaquin", 14, 10020);
        bc.push("Renato", "Joaquin", 13, 10020);
        bc.push("Renato", "Joaquin", 13, 10020);
        bc.push("Renato", "Joaquin", 13, 10020);

        bc.push("Renato", "Joaquin", 777, 10020);
        bc.push("Chachi", "Joaquin", 123, 100420);
        bc.push("Renato", "Joaquin", 3, 10020);
        bc.push("Renato", "Joaquin", 5, 10020);
        bc.push("Renato", "Joaquin", 1, 10020);
        bc.push("Renato", "Joaquin", 521, 10020);
        bc.push("Renato", "Joaquin", 14, 10020);
        bc.push("Renato", "Joaquin", 13, 10020);
        bc.push("Renato", "Joaquin", 13, 10020);
        bc.push("Renato", "Joaquin", 13, 10020);
        cout << bc << endl;
    };
    time_function(test_insert_entry, "test_insert_entry");


    string emisorHack, receptorHack;
    double montoHack;
    unsigned long long tiempoHack;
    cout << "Datos del entry para hackear\n---------------------------\n";
    cout << "Emisor: ";
    cin >> emisorHack;
    cout << "Receptor: ";
    cin >> receptorHack;
    cout << "Monto: ";
    cin >> montoHack;
    cout << "Tiempo(Numeros): ";
    cin >> tiempoHack;

    Entry entry(emisorHack, receptorHack, montoHack, tiempoHack);

    int blockHack, entryHack;
    cout << "\nPosicion a hackear\n---------------------\n";
    while (true) {
        cout << "Bloque (Numero menor o igual a " << bc.size() << " y mayor a 0): ";
        cin >> blockHack;
        if ((blockHack > 0 && blockHack <= bc.size())) { break; }
        cout << "Ingrese un numero valido\n";
    }
    cout << "Entry: ";
    cin >> entryHack;
    auto test_hack = [&]() {
        bc.hack(blockHack, entryHack, entry);
        cout << bc << endl;
    };
    time_function(test_hack, "test_hack");

    return 0;
}

template<typename Function, typename... Params>
void time_function(Function &fun, const string &function_name, Params &...params) {
    const auto start = std::chrono::steady_clock::now();
    fun(params...);
    const auto end = std::chrono::steady_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
    cout << "Executed function " << function_name << " in " << duration << " seconds." << endl;
}

void read_csv() {
    BlockChain<10> bc;
    string s;
    ifstream file("../MOCKDATA.csv");

    getline(file, s);
    while (true) {
        string emisor, receptor, monto, time;
        getline(file, emisor, ',');
        getline(file, receptor, ',');
        getline(file, monto, ',');
        getline(file, time);
        bc.push(emisor, receptor, stod(monto), stoll(time));
        if (file.eof()) break;
    }
    cout << bc << endl;
}
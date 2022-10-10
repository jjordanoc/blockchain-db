//
// Created by renato on 10/5/22.
//
#include "BlockChain.h"
#include <iostream>
#include "fstream"
using namespace std;

void read_csv();

int main()
{
    BlockChain<5> bc;

    auto start = std::chrono::steady_clock::now();
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

    auto end = std::chrono::steady_clock::now();
    double duration = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
    cout << "Se tardo " << duration << " segundos en insertar " << bc.size() << " bloques" << endl;

    cout << bc << endl;

    string emisorHack, receptorHack; double montoHack; unsigned long long tiempoHack;
    cout << "Datos del entry para hackear\n---------------------------\n";
    cout << "Emisor: ";cin >> emisorHack;
    cout << "Receptor: ";cin >> receptorHack;
    cout << "Monto: "; cin >> montoHack;
    cout << "Tiempo(Numeros): "; cin >> tiempoHack;

    Entry entry(emisorHack, receptorHack, montoHack, tiempoHack);

    int blockHack, entryHack;
    cout << "\nPosicion a hackear\n---------------------\n";
    while(true) {
        cout << "Bloque (Numero menor o igual a " << bc.size() << " y mayor a 0): ";cin >> blockHack;
        if((blockHack > 0 && blockHack <= bc.size())){break;}
        cout << "Ingrese un numero valido\n";
    }
    cout << "Entry: "; cin >> entryHack;
    start = std::chrono::steady_clock::now();
    bc.hack(blockHack, entryHack, entry);
    end = std::chrono::steady_clock::now();

    duration = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();

    cout << "Se tardo " << duration << " segundos el hackeo" << endl;

    cout << bc << endl;

    return 0;
}

void read_csv(){
    BlockChain<10> bc;
    string s;
    ifstream file("../MOCKDATA.csv");

    getline(file, s);
    while (true){
        string emisor, receptor, monto, time;
        getline(file, emisor, ',');
        getline(file, receptor, ',');
        getline(file, monto, ',');
        getline(file, time);
        bc.push(emisor, receptor, stod(monto), stoll(time));
        if(file.eof()) break;
    }
    cout << bc << endl;
}
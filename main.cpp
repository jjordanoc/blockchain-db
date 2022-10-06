//
// Created by renato on 10/5/22.
//
#include "BlockChain.h"
#include <iostream>
#include "fstream"
using namespace std;

int main()
{
    BlockChain<10> bc;
//    bc.push("Renato", "Joaquin", 777, 10020);
//    bc.push("Chachi", "Joaquin", 123, 100420);
//    bc.push("Renato", "Joaquin", 3, 10020);
//    bc.push("Renato", "Joaquin", 5, 10020);
//    bc.push("Renato", "Joaquin", 1, 10020);
//    bc.push("Renato", "Joaquin", 521, 10020);
//    bc.push("Renato", "Joaquin", 14, 10020);
//    bc.push("Renato", "Joaquin", 13, 10020);
//    bc.push("Renato", "Joaquin", 125, 10020);
//    bc.push("Renato", "Joaquin", 562, 10020);
//    bc.push("Renato", "Joaquin", 125, 10020);
//    bc.push("Renato", "Joaquin", 777, 10020);

    string s;
    ifstream file("MOCKDATA.csv");

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

    return 0;
}
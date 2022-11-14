//
// Created by renato on 10/5/22.
//
#include "blockchain.h"
#include "studententry.h"
#include "transactionentry.h"
#include <fstream>
#include <iostream>
using namespace std;

void csv_test(const string &filepath);
template<typename Function, typename... Params>
void time_function(Function &fun, const string &function_name, const Params &...params);

#define BLOCK_SIZE 5

int main() {
    BlockChain<BLOCK_SIZE> bc;

    auto test_insert_entry = [&]() {
        bc.insertEntry(new TransactionEntry("Renato", "Joaquin", 777, 10020));
        bc.insertEntry(new TransactionEntry("Chachi", "Joaquin", 123, 100420));
        bc.insertEntry(new TransactionEntry("Renato", "Joaquin", 3, 10020));
        bc.insertEntry(new TransactionEntry("Renato", "Joaquin", 5, 10020));
        bc.insertEntry(new TransactionEntry("Renato", "Joaquin", 1, 10020));
        bc.insertEntry(new TransactionEntry("Renato", "Joaquin", 521, 10020));
        bc.insertEntry(new TransactionEntry("Renato", "Joaquin", 14, 10020));
        bc.insertEntry(new TransactionEntry("Renato", "Joaquin", 13, 10020));
        bc.insertEntry(new TransactionEntry("Renato", "Joaquin", 13, 10020));
        bc.insertEntry(new TransactionEntry("Renato", "Joaquin", 13, 10020));

        bc.insertEntry(new TransactionEntry("Renato", "Joaquin", 777, 10020));
        bc.insertEntry(new TransactionEntry("Chachi", "Joaquin", 123, 100420));
        bc.insertEntry(new TransactionEntry("Renato", "Joaquin", 3, 10020));
        bc.insertEntry(new TransactionEntry("Renato", "Joaquin", 5, 10020));
        bc.insertEntry(new TransactionEntry("Renato", "Joaquin", 1, 10020));
        bc.insertEntry(new TransactionEntry("Renato", "Joaquin", 521, 10020));
        bc.insertEntry(new TransactionEntry("Renato", "Joaquin", 14, 10020));
        bc.insertEntry(new TransactionEntry("Renato", "Joaquin", 13, 10020));
        bc.insertEntry(new TransactionEntry("Renato", "Joaquin", 13, 10020));
        bc.insertEntry(new TransactionEntry("Renato", "Joaquin", 13, 10020));

        bc.insertEntry(new TransactionEntry("Renato", "Joaquin", 777, 10020));
        bc.insertEntry(new TransactionEntry("Chachi", "Joaquin", 123, 100420));
        bc.insertEntry(new TransactionEntry("Renato", "Joaquin", 3, 10020));
        bc.insertEntry(new TransactionEntry("Renato", "Joaquin", 5, 10020));
        bc.insertEntry(new TransactionEntry("Renato", "Joaquin", 1, 10020));
        bc.insertEntry(new TransactionEntry("Renato", "Joaquin", 521, 10020));
        bc.insertEntry(new TransactionEntry("Renato", "Joaquin", 14, 10020));
        bc.insertEntry(new TransactionEntry("Renato", "Joaquin", 13, 10020));
        bc.insertEntry(new TransactionEntry("Renato", "Joaquin", 13, 10020));
        bc.insertEntry(new TransactionEntry("Renato", "Joaquin", 13, 10020));
        bc.insertEntry(new StudentEntry("Joaquin", 20));
        bc.insertEntry(new StudentEntry("Renato", 20));
        bc.insertEntry(new StudentEntry("Jose Chachi", 20));
        cout << bc << endl;
        cout << "Is the blockchain valid? " << boolalpha << bc.isValid() << endl;
        bc.validate();
        cout << bc << endl;
        cout << bc.isValid() << endl;
    };
    time_function(test_insert_entry, "test_insert_entry");

    char is_hacking = 'N';
    cout << "Test hacking an entry? (Y/N): ";
    cin >> is_hacking;
    if (is_hacking == 'Y') {
        int blockHack, entryHack;
        cout << "Position to hack\n---------------------\n";
        while (true) {
            cout << "Block ID (Between " << bc.size() << " and 1): ";
            cin >> blockHack;
            if ((blockHack > 0 && blockHack <= bc.size())) { break; }
            cout << "Invalid number.\n";
        }
        cout << "Entry ID (Between " << BLOCK_SIZE << " and 1): ";
        cin >> entryHack;

        string emisorHack, receptorHack;
        double montoHack;
        time_t tiempoHack;
        auto currentEntry = bc.searchEntry(blockHack, entryHack);
        cout << "Current entry: " << (currentEntry == nullptr ? "No entry found at this position." : currentEntry->print()) << endl;
        cout << "New entry:\n------------------------------\n";
        cout << "Sender: ";
        cin >> emisorHack;
        cout << "Receiver: ";
        cin >> receptorHack;
        cout << "Amount: ";
        cin >> montoHack;
        cout << "Time (UNIX Timestamp): ";
        cin >> tiempoHack;

        auto entry = new TransactionEntry(emisorHack, receptorHack, montoHack, tiempoHack);

        auto test_hack = [&]() {
            bc.hackEntry(blockHack, entryHack, entry);
            cout << bc << endl;
            cout << "Is the blockchain valid? " << boolalpha << bc.isValid() << endl;
            bc.validate();
            cout << bc << endl;
            cout << "Is the blockchain valid? " << boolalpha << bc.isValid() << endl;
        };
        time_function(test_hack, "test_hack");
    }

    time_function(csv_test, "csv_test", "../MOCKDATA.csv");



    return 0;
}

template<typename Function, typename... Params>
void time_function(Function &fun, const string &function_name, const Params &...params) {
    cout << "Executing function " << function_name << "..." << endl;
    const auto start = std::chrono::steady_clock::now();
    fun(params...);
    const auto end = std::chrono::steady_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
    cout << "Executed function " << function_name << " in " << duration << " seconds." << endl;
}

void csv_test(const string &filepath) {
    BlockChain<10> bc;
    string s;
    ifstream file(filepath);

    getline(file, s);
    while (true) {
        string emisor, receptor, monto, time;
        getline(file, emisor, ',');
        getline(file, receptor, ',');
        getline(file, monto, ',');
        getline(file, time);
        bc.insertEntry(new TransactionEntry(emisor, receptor, stod(monto), stoll(time)));
        if (file.eof()) break;
    }
    cout << bc << endl;
    cout << "Is the blockchain valid? " << boolalpha << bc.isValid() << endl;
    bc.validate();
    cout << bc << endl;
    cout << "Is the blockchain valid? " << boolalpha << bc.isValid() << endl;
}
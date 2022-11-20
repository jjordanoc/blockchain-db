#include "mainwindow.h"
#include "blockchain.h"
#include "transactionentry.h"
#include "studententry.h"
#include "globals.h"
#include <QApplication>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

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
//    test_insert_entry();
    w.show();
    return a.exec();
}

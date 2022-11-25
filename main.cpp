#include "mainwindow.h"
#include "blockchain.h"
#include "transactionentry.h"
#include "studententry.h"
#include "globals.h"
#include <QApplication>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // paleta tomada de https://gist.github.com/QuantumCD/6245215
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(53,53,53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25,25,25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53,53,53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53,53,53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));

    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);
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
    a.setPalette(darkPalette);
    return a.exec();
}

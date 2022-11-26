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
    w.show();
    return a.exec();
}

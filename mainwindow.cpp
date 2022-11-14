#include "mainwindow.h"
#include "blockwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("QTextEdit");
    resize(500,400);
    mainView = new QGridLayout(this);
    BlockWidget *block = new BlockWidget(0, 1, "myHash", this);
    mainView->addWidget(block, 0, 1);
}

MainWindow::~MainWindow()
{
}


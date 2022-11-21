#include "mainwindow.h"
#include "blockwidget.h"
#include  <QCheckBox>
#include <QTextEdit>
#include <QVBoxLayout>
#include "createblockform.h"
#include <QPushButton>
#include <QDialog>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    setGeometry(0, 0, 1280, 720);
    blockChain = new BlockChain<BLOCK_SIZE>();
    scrollArea = new QScrollArea();
    scrollArea->setGeometry(0, 0, 1280, 720);
    mainView = new QHBoxLayout(this);
    mainView->setSizeConstraint(QHBoxLayout::SetMinAndMaxSize);
    auto *btn = new QPushButton("&Crear transacción", this);
    connect(btn, &QPushButton::clicked, this, &MainWindow::onCreateBlockButtonClick);
    mainView->addWidget(btn);
    scrollArea->setWidget(this);
    scrollArea->show();

}

MainWindow::~MainWindow()
{
    delete blockChain;
}

void MainWindow::onCreateBlockButtonClick()
{
    auto *dialog = new QDialog();
    dialog->setModal(true);
    dialog->setGeometry(0, 0, 400, 400);
    auto *createBlockForm = new CreateBlockForm(blockChain, dialog);
    connect(createBlockForm, SIGNAL(updatedBlockChain(Block<BLOCK_SIZE>*)), this, SLOT(redrawBlockChain(Block<BLOCK_SIZE>*)));
    createBlockForm->show();
    dialog->exec();
}

void MainWindow::redrawBlockChain(Block<BLOCK_SIZE> *block)
{
    std::cout << "Redrawing" << std::endl;
    auto *blockWidget = new BlockWidget(block, this);
    mainView->addWidget(blockWidget);
    cout << *blockChain << endl;
}


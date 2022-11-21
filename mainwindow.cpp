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
    blockChain = new BlockChain<BLOCK_SIZE>();
    scrollArea = new QScrollArea();
    scrollArea->setGeometry(0, 0, 1280, 720);
    mainView = new QHBoxLayout(this);
    auto *btn = new QPushButton("&Crear transacción", this);
    connect(btn, &QPushButton::clicked, this, &MainWindow::onCreateBlockButtonClick);
    mainView->addWidget(btn);
    for (int i = 0; i < 5; ++i) {
        auto *block = new BlockWidget(0, 0, "hola", this);
        mainView->addWidget(block);
    }
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
    connect(createBlockForm, SIGNAL(updatedBlockChain()), this, SLOT(redrawBlockChain()));
    createBlockForm->show();
    dialog->exec();
}

void MainWindow::redrawBlockChain()
{
    std::cout << "Redrawing" << std::endl;
    auto *tmp = blockChain->head;
    while (tmp->next != blockChain->head) {
        auto *block = new BlockWidget(tmp->data, this);
        mainView->addWidget(block);
        tmp = tmp->next;
    }
    cout << *blockChain << endl;
    mainView->update();
    this->repaint();
}


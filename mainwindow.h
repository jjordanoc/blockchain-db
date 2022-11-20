#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QScrollArea>
#include "blockchain.h"
#include "globals.h"


class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    int width = 1080;
    int height = 720;
    QHBoxLayout *mainView{};
    QScrollArea *scrollArea{};
    BlockChain<BLOCK_SIZE> *blockChain{};
    void onCreateBlockButtonClick();
private slots:
    void redrawBlockChain();
};
#endif // MAINWINDOW_H

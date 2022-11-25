#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QScrollArea>
#include "blockchain.h"
#include "globals.h"
#include <QWidget>

namespace Ui {
class MainWindow;
}


class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    Ui::MainWindow *ui;
    int width = 1080;
    int height = 720;
    QScrollArea *scrollArea{};
    BlockChain<BLOCK_SIZE> *blockChain{};
    void onCreateBlockButtonClick();
    BlockWidget *lastBlockInserted{};
private slots:
    void redrawBlockChain(Block<BLOCK_SIZE> *);
};
#endif // MAINWINDOW_H

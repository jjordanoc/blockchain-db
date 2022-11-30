#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QScrollArea>
#include "blockchain.h"
#include "compacttrie.h"
#include "globals.h"
#include <unordered_map>
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
    BlockChain<BLOCK_SIZE>::iterator blockChainIterator;
    void onCreateBlockButtonClick();
    void onCreateIndexButtonClick();
    void onCreateQueryButtonClick();
    BlockWidget *lastBlockInserted{};
    void redrawBlockChainOnFileUpload();
    template<typename T>
    void updateTime(TimedResult<T> &r);

    // Data Structures
    CompactTrie *compactTrie{};
private slots:
    void redrawBlockChain(Block<BLOCK_SIZE> *);
    void applyFilter(std::unordered_map<std::string, std::string> um);
    void uploadDataFromFile();
};
#endif // MAINWINDOW_H

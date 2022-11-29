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
    BlockChain<BLOCK_SIZE>::iterator blockChainIterator;
    void onCreateBlockButtonClick();
    BlockWidget *lastBlockInserted{};
    void redrawBlockChainOnFileUpload();
    void redrawBlockChainOnUpdate(int blockId, int entryId, Block<BLOCK_SIZE> *updatedBlock);
    template<typename T>
    void updateTime(TimedResult<T> &r);
    const QString dialogStyle = "background-color: rgb(50, 50, 75); color: white;";
    void clearBlockView();
private slots:
    void redrawBlockChain(Block<BLOCK_SIZE> *);
    void uploadDataFromFile();
    void onUpdateEntryButtonClick();
    void updateEntryAtPosition(int blockId, int entryId);
    void validateBlockChain();
};
#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QScrollArea>
#include <QFuture>
#include <QFutureWatcher>
#include <QWidget>

#include "globals.h"

#include "blockchain.h"
#include "compacttrie.h"
#include "avl.h"
#include "heap.h"
#include "hash.h"

#include <unordered_map>

using namespace std;

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
    QFutureWatcher<void> futureWatcher{};
    void redrawBlockChainOnFileUpload();
    void redrawBlockChainOnUpdate(int blockId, int entryId, Block<BLOCK_SIZE> *updatedBlock);
    template<typename T>
    void updateTime(TimedResult<T> &r);
    const QString dialogStyle = "background-color: rgb(50, 50, 75); color: white;";
    void clearBlockView();
    // indexes
    unordered_map<string, Index*> indexes;
private slots:
    void redrawBlockChain(Block<BLOCK_SIZE> *);
    void applyFilter(std::unordered_map<std::string, std::string> um);
    void uploadDataFromFile();
    void onUpdateEntryButtonClick();
    void updateEntryAtPosition(int blockId, int entryId);
    void deleteEntryAtPosition(int blockId, int entryId);
    void validateBlockChain();
    void redrawBlockChainAfterMine();
    void showWaitingIcon();
    void createIndexes(QString,QString);
    void onDeleteEntryButtonClick();
};
#endif // MAINWINDOW_H

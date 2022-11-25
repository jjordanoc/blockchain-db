#ifndef BLOCKWIDGET_H
#define BLOCKWIDGET_H

#include "blockidinputbox.h"
#include <QTextEdit>
#include <QVBoxLayout>
#include<QWidget>
#include "block.h"
#include "globals.h"
#include <QLabel>

namespace Ui {
class BlockWidget;
}

class BlockWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BlockWidget(Block<BLOCK_SIZE> *_block, QWidget *parent);
    void updateBlockData();
signals:
private:
//    QLabel *id{};
//    QLabel *nonce{};
//    QLabel *hashCode{};
//    QLabel *prev{};
//    QLabel *blockData{};
    Block <BLOCK_SIZE> *block{};
    Ui::BlockWidget *ui{};
    int localFillCount = 0;
//    QVBoxLayout *layout{};
};

#endif // BLOCKWIDGET_H

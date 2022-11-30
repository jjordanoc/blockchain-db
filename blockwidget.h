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
    Block <BLOCK_SIZE> *block{};
    void refreshViewOnUpdate(int entryId);
    void changeStyle(const QString &style);
private:
    Ui::BlockWidget *ui{};
    int localFillCount = 0;
};

#endif // BLOCKWIDGET_H

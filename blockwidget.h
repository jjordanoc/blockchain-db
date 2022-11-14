#ifndef BLOCKWIDGET_H
#define BLOCKWIDGET_H

#include "blockidinputbox.h"
#include <QTextEdit>
#include <QVBoxLayout>
#include<QWidget>

class BlockWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BlockWidget(const size_t &îd, const size_t &nonce, const std::string &hashCode, QWidget *parent);
signals:
private:
    BlockInputBox *id;
    BlockInputBox *nonce;
    BlockInputBox *hashCode;
    BlockInputBox *prev;
    QVBoxLayout *layout;
};

#endif // BLOCKWIDGET_H

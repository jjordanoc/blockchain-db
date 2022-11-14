#ifndef BLOCKIDINPUTBOX_H
#define BLOCKIDINPUTBOX_H

#include <QTextEdit>
#include <QWidget>

class BlockInputBox : public QTextEdit
{
    Q_OBJECT
public:
    explicit BlockInputBox(const QString &text, QWidget *parent = nullptr);
};

#endif // BLOCKIDINPUTBOX_H

#include "blockidinputbox.h"

BlockInputBox::BlockInputBox(const QString &text, QWidget *parent)
: QTextEdit(parent)
{
    setText(text);
}

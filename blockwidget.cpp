#include "blockwidget.h"
#include "qlocale.h"


BlockWidget::BlockWidget(const size_t &îd, const size_t &nonce, const std::string &hashCode, QWidget *parent)
    : QWidget(parent)
{
    QString qHashCode = QString::fromStdString(hashCode);
    this->id = new BlockInputBox(qHashCode, this);
    this->hashCode = new BlockInputBox(tr("my hash code"), this);
    this->nonce = new BlockInputBox(tr("my nonce"), this);

    this->layout = new QVBoxLayout(this);
    this->layout->addWidget(this->id);
    this->layout->addWidget(this->hashCode);
    this->layout->addWidget(this->nonce);
}

#include "blockwidget.h"
#include "qlocale.h"


BlockWidget::BlockWidget(const size_t &id, const size_t &nonce, const std::string &hashCode, QWidget *parent)
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

BlockWidget::BlockWidget(Block<BLOCK_SIZE> *block, QWidget *parent)
{
    QString qId = QString::fromStdString(block->getId());
    auto *id = new BlockInputBox(qId, this);
    QString qNonce = QString::fromStdString(block->getNonce());
    auto *nonce = new BlockInputBox(qNonce, this);
    QString qPrev = QString::fromStdString(*block->getPrev());
    auto *prev = new BlockInputBox(qPrev, this);
    QString qHashCode = QString::fromStdString(*block->getHashCode());
    auto *hashCode = new BlockInputBox(qHashCode, this);
    QString qData = QString::fromStdString(block->getData());
    auto *data = new BlockInputBox(qData, this);

    this->layout = new QVBoxLayout(this);

    this->layout->addWidget(id);
    this->layout->addWidget(nonce);
    this->layout->addWidget(data);
    this->layout->addWidget(prev);
    this->layout->addWidget(hashCode);
}

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

BlockWidget::BlockWidget(Block<BLOCK_SIZE> *_block, QWidget *parent)
: block(_block), QWidget(parent)
{
    QString qId = QString::fromStdString(block->getId());
    id = new BlockInputBox(qId, this);
    QString qNonce = QString::fromStdString(block->getNonce());
    nonce = new BlockInputBox(qNonce, this);
    QString qPrev = QString::fromStdString(*block->getPrev());
    prev = new BlockInputBox(qPrev, this);
    QString qHashCode = QString::fromStdString(*block->getHashCode());
    hashCode = new BlockInputBox(qHashCode, this);
    QString qData = QString::fromStdString(block->getData());
    blockData = new BlockInputBox(qData, this);

    this->layout = new QVBoxLayout(this);

    this->layout->addWidget(id);
    this->layout->addWidget(nonce);
    this->layout->addWidget(blockData);
    this->layout->addWidget(prev);
    this->layout->addWidget(hashCode);
}

void BlockWidget::updateBlockData()
{
    cout << "New data:" << block->getData() << endl;
    QString newData = QString::fromStdString(block->getData());
    blockData->setText(newData);
}

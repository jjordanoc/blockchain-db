#include "blockwidget.h"
#include "qlocale.h"
#include "ui_blockwidget.h"
#include "entrywidget.h"


BlockWidget::BlockWidget(Block<BLOCK_SIZE> *_block, QWidget *parent)
    : block(_block), QWidget(parent), ui(new Ui::BlockWidget)
{
    ui->setupUi(this);
    QString qId = QString::fromStdString(block->getId());
    ui->blockId->setText(qId);
    QString qNonce = QString::fromStdString(block->getNonce());
    ui->nonce->setText(qNonce);
    QString qPrev = QString::fromStdString(*block->getPrev());
    ui->prev->setText(qPrev);
    QString qHashCode = QString::fromStdString(*block->getHashCode());
    ui->hash->setText(qHashCode);
    this->localFillCount = block->getFillCount();
    for (int i = 0; i < this->localFillCount; ++i) {
        EntryWidget *entry = new EntryWidget(i, block->datos[i]);
        ui->entryDiv->addWidget(entry);
    }
}

void BlockWidget::updateBlockData()
{
    for (int i = this->localFillCount; i < block->getFillCount(); ++i) {
        EntryWidget *entry = new EntryWidget(i, block->datos[i]);
        ui->entryDiv->addWidget(entry);
    }
    this->localFillCount = block->getFillCount();
}

void BlockWidget::refreshViewOnUpdate(int entryId)
{
//    for (int i = 0; i < this->localFillCount; ++i) {
//        auto *entryWidget = qobject_cast<EntryWidget*>(ui->entryDiv->itemAt(i)->widget());
//        if (entryWidget != nullptr) {
//            entryWidget->deleteLater();
//        }
//        else {
//            cout << "Error" << endl;
//        }
//    }
//    QString qId = QString::fromStdString(block->getId());
//    ui->blockId->setText(qId);
//    QString qNonce = QString::fromStdString(block->getNonce());
//    ui->nonce->setText(qNonce);
//    QString qPrev = QString::fromStdString(*block->getPrev());
//    ui->prev->setText(qPrev);
//    QString qHashCode = QString::fromStdString(*block->getHashCode());
//    ui->hash->setText(qHashCode);
//    this->localFillCount = block->getFillCount();
//    for (int i = 0; i < this->localFillCount; ++i) {
//        EntryWidget *entry = new EntryWidget(i, block->datos[i]);
//        ui->entryDiv->addWidget(entry);
//    }
    auto *entryWidget = qobject_cast<EntryWidget*>(ui->entryDiv->itemAt(entryId-1)->widget());
    entryWidget->updateData(entryId, block->getEntry(entryId));
}

void BlockWidget::changeStyle(const QString &style)
{
    ui->verticalDiv->setStyleSheet(style);
}

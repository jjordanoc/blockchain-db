#include "entrywidget.h"
#include "ui_entrywidget.h"
#include "transactionentry.h"

EntryWidget::EntryWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EntryWidget)
{
    ui->setupUi(this);
}

EntryWidget::EntryWidget(int entryId, Entry *entry, QWidget *parent)
    :     QWidget(parent),
      ui(new Ui::EntryWidget)
{
    ui->setupUi(this);
    QString qEntryId = QString::number(entryId);
    ui->id->setText(qEntryId);
    QString qAmount = QString::number(dynamic_cast<TransactionEntry *>(entry)->monto);
    ui->amount->setText(qAmount);
    QString qDate = QString::fromStdString(dynamic_cast<TransactionEntry *>(entry)->getDate());
    ui->date->setText(qDate);
    QString qReciever = QString::fromStdString(dynamic_cast<TransactionEntry *>(entry)->receptor);
    ui->receiver->setText(qReciever);
    QString qSender = QString::fromStdString(dynamic_cast<TransactionEntry *>(entry)->emisor);
    ui->sender->setText(qSender);
}

void EntryWidget::updateData(int entryId, Entry *entry)
{
    QString qEntryId = QString::number(entryId);
    ui->id->setText(qEntryId);
    QString qAmount = QString::number(dynamic_cast<TransactionEntry *>(entry)->monto);
    ui->amount->setText(qAmount);
    QString qDate = QString::fromStdString(dynamic_cast<TransactionEntry *>(entry)->getDate());
    ui->date->setText(qDate);
    QString qReciever = QString::fromStdString(dynamic_cast<TransactionEntry *>(entry)->receptor);
    ui->receiver->setText(qReciever);
    QString qSender = QString::fromStdString(dynamic_cast<TransactionEntry *>(entry)->emisor);
    ui->sender->setText(qSender);
}

EntryWidget::~EntryWidget()
{
    delete ui;
}

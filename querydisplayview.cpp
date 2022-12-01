#include "querydisplayview.h"
#include "ui_querydisplayview.h"
#include "transactionentry.h"


QueryDisplayView::QueryDisplayView(std::vector<Entry*>& entries, std::unordered_map<std::string, std::string>& um, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QueryDisplayView)
{
    ui->setupUi(this);

    auto* table = ui->tableWidget;
    table->setRowCount(entries.size());

    ui->filtro->setText(QString::fromStdString(um["filter"]));
    ui->tipo->setText(QString::fromStdString(um["type"]));
    ui->valor->setText(QString::fromStdString(um["value1"]));

    for(int i = 0; i < entries.size(); ++i)
    {
        cout << i << endl;
        auto* entry = (TransactionEntry*)(entries[i]);
        table->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(entry->emisor)));
        table->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(entry->receptor)));
        table->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(to_string(entry->monto))));
        table->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(entry->getDate())));
    }
}

QueryDisplayView::QueryDisplayView(const string &usuario, double aggregate, QWidget *parent) : QWidget(parent), ui(new Ui::QueryDisplayView) {
     ui->setupUi(this);

     auto* table = ui->tableWidget;
     table->setRowCount(1);

     ui->filtro->setText(tr(""));
     ui->valor->setText(tr(""));
     ui->tipo->setText(tr("Monto total"));

     table->setItem(0, 0, new QTableWidgetItem(QString::fromStdString(usuario)));
     table->setItem(0, 1, new QTableWidgetItem(tr("-")));
     table->setItem(0, 2, new QTableWidgetItem(QString::number(aggregate)));
     table->setItem(0, 3, new QTableWidgetItem(tr("-")));
}

QueryDisplayView::~QueryDisplayView()
{
    delete ui;
}

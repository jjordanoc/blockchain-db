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

    cout << "Colocando la tabla...." << endl;
    for(int i = 0; i < entries.size(); ++i)
    {
        cout << i << endl;
        auto* entry = dynamic_cast<TransactionEntry*>(entries[i]);
        table->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(entry->emisor)));
        table->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(entry->receptor)));
        table->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(to_string(entry->monto))));
        table->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(entry->getDate())));
    }
    cout << "Se termino de colocar la tabla...." << endl;
}

QueryDisplayView::~QueryDisplayView()
{
    delete ui;
}

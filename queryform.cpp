#include "queryform.h"
#include "ui_queryform.h"

QueryForm::QueryForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QueryForm)
{
    ui->setupUi(this);
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(submitForm()));
}

void QueryForm::submitForm()
{
    std::cout << "Submitted form" << std::endl;
    std::string filter = ui->filtro->currentText().toStdString();
    std::string type = ui->tipo->currentText().toStdString();
    std::string value1 = ui->texto->text().toStdString();
    std::string value2 = ui->lineEdit->text().toStdString();
    time_t datetime1 = ui->dateTimeEdit->dateTime().toSecsSinceEpoch();
    time_t datetime2 = ui->dateTimeEdit_2->dateTime().toSecsSinceEpoch();

    std::cout << "filtrar por: " << filter << std::endl;
    std::cout << "tipo: " << type << std::endl;
    std::cout << "valor: " << value1 << std::endl;

    std::unordered_map<std::string, std::string> um = {
        std::make_pair("filter", filter),
        std::make_pair("type",type),
        std::make_pair("value1", value1),
        std::make_pair("value2", value2),
        std::make_pair("datetime1", std::to_string(datetime1)),
        std::make_pair("datetime2", std::to_string(datetime2))
    };

    emit emitData(um);
}

QueryForm::~QueryForm()
{
    delete ui;
}

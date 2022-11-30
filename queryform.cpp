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
    std::string value = ui->texto->text().toStdString();
    std::cout << "filtrar por: " << filter << std::endl;
    std::cout << "tipo: " << type << std::endl;
    std::cout << "valor: " << value << std::endl;

    std::unordered_map<std::string, std::string> um = {std::make_pair("filter", filter), std::make_pair("type",type), std::make_pair("value1", value)};
    emit emitData(um);
}

QueryForm::~QueryForm()
{
    delete ui;
}

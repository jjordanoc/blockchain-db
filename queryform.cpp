#include "queryform.h"
#include "ui_queryform.h"

QueryForm::QueryForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QueryForm)
{
    ui->setupUi(this);
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(submitForm()));
    connect(ui->filtro,SIGNAL(currentTextChanged(const QString&)),this,SLOT(updateInputsOnFiltroChange(const QString&)));
    connect(ui->tipo,SIGNAL(currentTextChanged(const QString&)),this,SLOT(updateInputsOnTipoChange(const QString&)));
    ui->dateTimeEdit->hide();
    ui->dateTimeEdit_2->hide();
    ui->lineEdit->hide();
    ui->label_2->hide();
    ui->montoCheckBox->show();
    this->currentFiltro = "Emisor";
    this->currentTipo = "Igual";
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
    bool aggregate = ui->montoCheckBox->isChecked();
    std::cout << "filtrar por: " << filter << std::endl;
    std::cout << "tipo: " << type << std::endl;
    std::cout << "valor: " << value1 << std::endl;
    std::cout << "agregar: " << std::to_string(aggregate) << std::endl;

    std::unordered_map<std::string, std::string> um = {
        std::make_pair("filter", filter),
        std::make_pair("type",type),
        std::make_pair("value1", value1),
        std::make_pair("value2", value2),
        std::make_pair("datetime1", std::to_string(datetime1)),
        std::make_pair("datetime2", std::to_string(datetime2)),
        std::make_pair("aggregate", std::to_string(aggregate))
    };

    emit emitData(um);
}

void QueryForm::updateInputsOnFiltroChange(const QString &filtro)
{
    this->currentFiltro = filtro;
    this->updateUiOnAnyChange();
}

void QueryForm::updateInputsOnTipoChange(const QString &tipo)
{
    this->currentTipo = tipo;
    this->updateUiOnAnyChange();
}

void QueryForm::updateUiOnAnyChange()
{
    std::cout << "updating ui for tipo:" << currentTipo.toStdString() << " filtro " << currentFiltro.toStdString() << std::endl;
    if (currentTipo == "Entre"){
        ui->label_2->show();
        if (currentFiltro == "Fecha") {
            ui->dateTimeEdit->show();
            ui->dateTimeEdit_2->show();
            ui->texto->hide();
            ui->lineEdit->hide();
        }
        else {
            ui->dateTimeEdit->hide();
            ui->dateTimeEdit_2->hide();
            ui->texto->show();
            ui->lineEdit->show();
        }
        ui->montoCheckBox->hide();
        ui->montoCheckBox->setCheckState(Qt::CheckState::Unchecked);
    }
    else if (currentTipo == "Maximo" || currentTipo == "Minimo") {
        ui->label_2->hide();
        ui->dateTimeEdit_2->hide();
        ui->lineEdit->hide();
        ui->texto->hide();
        ui->dateTimeEdit->hide();
        ui->montoCheckBox->hide();
        ui->montoCheckBox->setCheckState(Qt::CheckState::Unchecked);
    }
    else {
        if (currentTipo == "Igual" && (currentFiltro == "Receptor" || currentFiltro == "Emisor")) {
            ui->montoCheckBox->show();
        }
        else {
            ui->montoCheckBox->hide();
            ui->montoCheckBox->setCheckState(Qt::CheckState::Unchecked);
        }
        ui->label_2->hide();
        ui->dateTimeEdit_2->hide();
        ui->lineEdit->hide();
        if (currentFiltro == "Fecha") {
            ui->dateTimeEdit->show();
            ui->texto->hide();
        }
        else {
            ui->dateTimeEdit->hide();
            ui->texto->show();
        }
    }
}

QueryForm::~QueryForm()
{
    delete ui;
}

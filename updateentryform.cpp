#include "updateentryform.h"
#include "ui_updateentryform.h"

UpdateEntryForm::UpdateEntryForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UpdateEntryForm)
{
    ui->setupUi(this);
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(submitForm()));
}

UpdateEntryForm::~UpdateEntryForm()
{
    delete ui;
}

void UpdateEntryForm::submitForm()
{
    QString emisor = ui->emisorTextEdit->toPlainText();
    QString receptor = ui->receptorTextEdit->toPlainText();
    QDateTime fecha = ui->fechaTimeEdit->dateTime();

    std::string emisorStd = emisor.toStdString();
    std::string receptorStd = receptor.toStdString();
    time_t fechaUnix = fecha.toSecsSinceEpoch();
    double monto = ui->montoSpinBox->value();
    cout << emisorStd << endl << receptorStd <<  endl  << fechaUnix << endl << monto << endl;

    emit updatedEntryValue(new TransactionEntry(emisorStd, receptorStd, monto, fechaUnix));
}

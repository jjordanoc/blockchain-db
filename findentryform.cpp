#include "findentryform.h"
#include "ui_findentryform.h"

FindEntryForm::FindEntryForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FindEntryForm)
{
    ui->setupUi(this);
    connect(ui->buttonBox, SIGNAL(accepted()), SLOT(submitForm()));
}

FindEntryForm::~FindEntryForm()
{
    delete ui;
}

void FindEntryForm::submitForm()
{
    int blockId = ui->blockIdSpinBox->value();
    int entryId = ui->entryIdSpinBox->value();
    emit foundEntry(blockId, entryId);
}

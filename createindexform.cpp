#include "createindexform.h"
#include "ui_createindexform.h"
#include <unordered_map>

CreateIndexForm::CreateIndexForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CreateIndexForm)
{
    ui->setupUi(this);
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(submitForm()));
}

void CreateIndexForm::submitForm()
{
    std::cout << "Submitted form" << std::endl;
    QString structure = ui->structure->currentText();
    QString attribute = ui->atribute->currentText();
    std::cout << "Struct: " << structure.toStdString() << endl;
    std::cout << "Attribute: " << attribute.toStdString() << endl;
    emit submittedForm(structure, attribute);
}

CreateIndexForm::~CreateIndexForm()
{
    delete ui;
}

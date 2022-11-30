#ifndef CREATEINDEXFORM_H
#define CREATEINDEXFORM_H
#include "globals.h"
#include "blockchain.h"
#include "compacttrie.h"
#include <QWidget>

namespace Ui {
class CreateIndexForm;
}

class CreateIndexForm : public QWidget
{
    Q_OBJECT

public:
    explicit CreateIndexForm(QWidget *parent = nullptr);
    ~CreateIndexForm();
private slots:
    void submitForm();
private:
    Ui::CreateIndexForm *ui;
signals:
    void submittedForm(QString, QString);
};

#endif // CREATEINDEXFORM_H

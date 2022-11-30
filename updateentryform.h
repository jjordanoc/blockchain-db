#ifndef UPDATEENTRYFORM_H
#define UPDATEENTRYFORM_H

#include <QWidget>
#include "transactionentry.h"

namespace Ui {
class UpdateEntryForm;
}

class UpdateEntryForm : public QWidget
{
    Q_OBJECT

public:
    explicit UpdateEntryForm(QWidget *parent = nullptr);
    ~UpdateEntryForm();

private:
    Ui::UpdateEntryForm *ui;
private slots:
    void submitForm();
signals:
    void updatedEntryValue(Entry *newEntry);
};

#endif // UPDATEENTRYFORM_H

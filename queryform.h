#ifndef QUERYFORM_H
#define QUERYFORM_H

#include "globals.h"
#include <unordered_map>
#include <QWidget>

namespace Ui {
class QueryForm;
}

class QueryForm : public QWidget
{
    Q_OBJECT

public:
    explicit QueryForm(QWidget *parent = nullptr);
    ~QueryForm();
private:
    Ui::QueryForm *ui;
    QString currentFiltro;
    QString currentTipo;
private slots:
    void submitForm();
    void updateInputsOnFiltroChange(const QString &);
    void updateInputsOnTipoChange(const QString &);
    void updateUiOnAnyChange();
signals:
    void emitData(std::unordered_map<std::string, std::string>);
};

#endif // QUERYFORM_H

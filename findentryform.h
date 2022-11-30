#ifndef FINDENTRYFORM_H
#define FINDENTRYFORM_H

#include <QWidget>

namespace Ui {
class FindEntryForm;
}

class FindEntryForm : public QWidget
{
    Q_OBJECT

public:
    explicit FindEntryForm(QWidget *parent = nullptr);
    ~FindEntryForm();

private:
    Ui::FindEntryForm *ui;
private slots:
    void submitForm();
signals:
    void foundEntry(int blockId, int entryId);
};

#endif // FINDENTRYFORM_H

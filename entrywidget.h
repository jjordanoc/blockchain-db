#ifndef ENTRYWIDGET_H
#define ENTRYWIDGET_H

#include <QWidget>
#include "entry.h"

namespace Ui {
class EntryWidget;
}

class EntryWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EntryWidget(QWidget *parent = nullptr);
    explicit EntryWidget(int entryId, Entry *entry, QWidget *parent = nullptr);
    void updateData(int entryId, Entry *entry);
    ~EntryWidget();

private:
    Ui::EntryWidget *ui;
};

#endif // ENTRYWIDGET_H

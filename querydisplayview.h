#ifndef QUERYDISPLAYVIEW_H
#define QUERYDISPLAYVIEW_H

#include <QWidget>
#include "entry.h"
#include <vector>

namespace Ui {
class QueryDisplayView;
}

class QueryDisplayView : public QWidget
{
    Q_OBJECT

public:
    explicit QueryDisplayView(std::vector<Entry*>& entries, std::unordered_map<std::string, std::string>& um, QWidget *parent = nullptr);
    explicit QueryDisplayView(const string &usuario, double aggregate, QWidget *parent = nullptr);
    ~QueryDisplayView();

private:
    Ui::QueryDisplayView *ui;
};

#endif // QUERYDISPLAYVIEW_H

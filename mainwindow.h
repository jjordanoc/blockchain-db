#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QScrollArea>

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    int width = 1080;
    int height = 720;
    QHBoxLayout *mainView;
    QScrollArea *scrollArea;
};
#endif // MAINWINDOW_H

#include "mainwindow.h"
#include "blockwidget.h"
#include  <QCheckBox>
#include <QTextEdit>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    // setup scroll area
    scrollArea = new QScrollArea();
    scrollArea->setGeometry(0, 0, 300, 300);
//    resize(700,500);
    mainView = new QHBoxLayout(this);

//    BlockWidget **blocks = new BlockWidget*[100];
    for (int i = 0; i < 100; ++i) {
        auto *sub = new QWidget();
        auto *hboxSub = new QVBoxLayout(sub);
        for (int i = 0; i < 10; ++i) {
            QTextEdit *edit = new QTextEdit();
            edit->setText(tr("hash"));
            hboxSub->addWidget(edit);
        }
        mainView->addWidget(sub);
    }
//    QScrollArea *scrl = new QScrollArea();
//        scrl->setGeometry(0,0,300,300);
//        QWidget *wgtMain = new QWidget();
//        QHBoxLayout *hboxMain = new QHBoxLayout(wgtMain);
//        for(int iCount=0;iCount<3;iCount++){
//            QWidget *wgtSub = new QWidget();
//            QVBoxLayout *vboxSub = new QVBoxLayout(wgtSub);
//            for(int jCount=0;jCount<10;jCount++){
//                QCheckBox *chk = new QCheckBox("Check Box " + QString::number(jCount+1) + " of " + QString::number(iCount+1));
//                vboxSub->addWidget(chk);
//            }
//            hboxMain->addWidget(wgtSub);
//        }
//        scrl->setWidget(wgtMain);
//        scrl->show();
    scrollArea->setWidget(this);
    scrollArea->show();
}

MainWindow::~MainWindow()
{
}


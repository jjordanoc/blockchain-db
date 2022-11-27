#include "mainwindow.h"
#include "blockwidget.h"
#include  <QCheckBox>
#include <QTextEdit>
#include <QVBoxLayout>
#include "createblockform.h"
#include <QPushButton>
#include <QDialog>
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QStringList>
#include "transactionentry.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::MainWindow), blockChain(new BlockChain<BLOCK_SIZE>), blockChainIterator(blockChain->begin())
{
    ui->setupUi(this);
    connect(ui->createEntryButton, &QPushButton::clicked, this, &MainWindow::onCreateBlockButtonClick);
    connect(ui->uploadFileButton, SIGNAL(clicked()), this, SLOT(uploadDataFromFile()));
    ui->scrollArea->setWidget(ui->blockScrollAreaWidget);
}

MainWindow::~MainWindow()
{
    delete blockChain;
    delete ui;
}

void MainWindow::onCreateBlockButtonClick()
{
    auto *dialog = new QDialog();
    dialog->setModal(true);
    dialog->setGeometry(0, 0, 400, 400);
    auto *createBlockForm = new CreateBlockForm(blockChain, dialog);
    connect(createBlockForm, SIGNAL(updatedBlockChain(Block<BLOCK_SIZE>*)), this, SLOT(redrawBlockChain(Block<BLOCK_SIZE>*)));
    createBlockForm->show();
    dialog->exec();
}

void MainWindow::redrawBlockChainOnFileUpload()
{
    auto *mainView = ui->horizontalBlockDiv;
    if (blockChainIterator == this->blockChain->end()) {
        blockChainIterator = this->blockChain->begin();
    }
    else {
        ++blockChainIterator;
    }
    while (blockChainIterator != this->blockChain->end()) {
        cout << "Redrawing..." << endl;
        auto *block = *blockChainIterator;
        auto *blockWidget = new BlockWidget(block, this);
        mainView->addWidget(blockWidget);
        this->lastBlockInserted = blockWidget;
        ++blockChainIterator;
    }
    --blockChainIterator;
}

void MainWindow::uploadDataFromFile()
{
    cout << "Browsing files..." << endl;
    QString filename = QFileDialog::getOpenFileName(this, tr("Open csv"), QDir::currentPath(), tr("CSV Files (*.csv)"));
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    auto upload_file = [&]() {
        QTextStream in(&file);
        // skip first line
        in.readLine();
        while (!in.atEnd()) {
            QString line = in.readLine();
            cout << line.toStdString() << endl;
            QStringList args = line.split(",");
            if (args.size() == 4) {
                cout << "Parsing line..." << endl;
                QString qEmisor = args.at(0);
                QString qReceptor = args.at(1);
                QString qMonto = args.at(2);
                QString qFecha = args.at(3);
                std::string emisor = qEmisor.toStdString();
                std::string receptor = qReceptor.toStdString();
                double monto = qMonto.toDouble();
                time_t fecha = qFecha.toLongLong() / 1000;
                auto oldSize = blockChain->size();
                this->blockChain->insertEntry(new TransactionEntry(emisor, receptor, monto, fecha));
            }
        }
        this->redrawBlockChainOnFileUpload();
    };
    TimedResult r = time_function(upload_file);
    this->updateTime(r);
}

void MainWindow::redrawBlockChain(Block<BLOCK_SIZE> *block)
{
    auto *mainView = ui->horizontalBlockDiv;
    std::cout << "Redrawing" << std::endl;
    if (block != nullptr) {
        std::cout << "Creating new block." << std::endl;
        auto insert_block_widget = [&](){
            auto *blockWidget = new BlockWidget(block, this);
            mainView->addWidget(blockWidget);
            this->lastBlockInserted = blockWidget;
        };
        TimedResult r = time_function(insert_block_widget);
        this->updateTime(r);
    }
    else {
        this->lastBlockInserted->updateBlockData();
    }
    cout << *blockChain << endl;
}


template<typename T>
void MainWindow::updateTime(TimedResult<T> &r)
{
    QString timeText = QString::number(r.duration) + " ms";
    ui->timeLabel->setText(timeText);
}

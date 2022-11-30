#include "mainwindow.h"
#include "blockwidget.h"
#include  <QCheckBox>
#include <QTextEdit>
#include <QVBoxLayout>
#include "createblockform.h"
#include "createindexform.h"
#include "queryform.h"
#include "querydisplayview.h"
#include <QPushButton>
#include <QDialog>
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QStringList>
#include <QTableWidget>
#include <memory.h>
#include "transactionentry.h"


MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::MainWindow), blockChain(new BlockChain<BLOCK_SIZE>), blockChainIterator(blockChain->begin())
{
    ui->setupUi(this);
    connect(ui->createEntryButton, &QPushButton::clicked, this, &MainWindow::onCreateBlockButtonClick);
    connect(ui->createIndexButton, &QPushButton::clicked, this, &MainWindow::onCreateIndexButtonClick);
    connect(ui->queryButton, &QPushButton::clicked, this, &MainWindow::onCreateQueryButtonClick);
    connect(ui->uploadFileButton, SIGNAL(clicked()), this, SLOT(uploadDataFromFile()));
    ui->scrollArea->setWidget(ui->blockScrollAreaWidget);
}

MainWindow::~MainWindow()
{
    if(compactTrie != nullptr) delete compactTrie;
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

void MainWindow::onCreateIndexButtonClick()
{
    auto *dialog = new QDialog();
    dialog->setModal(true);
    dialog->setGeometry(0, 0, 400, 400);
    cout << "Creating Index Form..." << endl;
    auto *createIndexForm = new CreateIndexForm(blockChain, compactTrie, dialog);
    createIndexForm->show();
    dialog->exec();
}

void MainWindow::onCreateQueryButtonClick()
{
    auto *dialog = new QDialog();
    dialog->setModal(true);
    dialog->setGeometry(0, 0, 400, 400);
    cout << "Creating Query Form..." << endl;
    auto *createQueryForm = new QueryForm(dialog);
    connect(createQueryForm, SIGNAL(emitData(std::unordered_map<std::string, std::string>)), this, SLOT(applyFilter(std::unordered_map<std::string, std::string>)));
    createQueryForm->show();
    dialog->exec();
}

void MainWindow::applyFilter(std::unordered_map<std::string, std::string> um)
{
    cout << "applying filter..." << endl;
    for (auto& [a, b] : um) {
        cout << a << ": " <<b << endl;
    }

    auto filter = um["filter"];
    auto type = um["type"];
    auto value1 = um["value1"];

    std::vector<Entry*> result;
    auto bcIter = blockChain->begin();
    if(filter == "Receptor" || filter == "Emisor")
    {
        if(type == "Igual")
        {
            cout << "Equals..." << endl;
            if(compactTrie == nullptr){
                cout << "No compact Trie..." << endl;
                while(bcIter != blockChain->end())
                {
                    cout << "While1....." << endl;
                    cout << (*bcIter)->getData() << endl;
                    Entry** entries = (*bcIter)->getEntries();
                    for(int i = 0; i < (*bcIter)->getSize(); ++i)
                    {
                        cout << entries[i]->print() << endl;

                        if(filter == "Emisor" && dynamic_cast<TransactionEntry*>(entries[i])->emisor == value1){
                            result.push_back(entries[i]);
                        }
                        else if(filter == "Receptor" && dynamic_cast<TransactionEntry*>(entries[i])->receptor == value1){
                            result.push_back(entries[i]);
                        }
                    }
                    ++bcIter;
                }
            }
        }
        else if (type == "Contiene")
        {
            cout << "Contain..." << endl;
        }
    }
    // Impreza
    cout << "Termino el while" << endl;
    for(auto e : result)
    {
        cout << e->print() << endl;
    }

    cout << "Showing dialog..." << endl;
    auto *dialog = new QDialog();
    dialog->setModal(true);
    dialog->setGeometry(0, 0, 550, 400);
    auto* querydisplayview = new QueryDisplayView(result, um, dialog);
    querydisplayview->show();
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

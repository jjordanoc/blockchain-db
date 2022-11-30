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
#include "findentryform.h"
#include "updateentryform.h"
#include <QPainter>
#include <QStyleOption>
#include <QFuture>
#include <QtConcurrent>
#include <limits>



MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::MainWindow), blockChain(new BlockChain<BLOCK_SIZE>), blockChainIterator(blockChain->begin())
{
    ui->setupUi(this);
    connect(ui->createEntryButton, &QPushButton::clicked, this, &MainWindow::onCreateBlockButtonClick);
    connect(ui->createIndexButton, &QPushButton::clicked, this, &MainWindow::onCreateIndexButtonClick);
    connect(ui->queryButton, &QPushButton::clicked, this, &MainWindow::onCreateQueryButtonClick);
    connect(ui->uploadFileButton, SIGNAL(clicked()), this, SLOT(uploadDataFromFile()));
    connect(ui->modifyEntryButton, SIGNAL(clicked()), this, SLOT(onUpdateEntryButtonClick()));
    connect(ui->mineButton, SIGNAL(clicked()), this, SLOT(validateBlockChain()));
    connect(&this->futureWatcher, &QFutureWatcher<void>::finished, this, &MainWindow::redrawBlockChainAfterMine);
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
    dialog->setStyleSheet(dialogStyle);
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
    auto value2 = um["value2"]; // 2do parametro del entre
    auto datetime1 = stoull(um["datetime1"]);
    auto datetime2 = stoull(um["datetime2"]); // 2do parametro del entre

    std::vector<Entry*> result;
    auto bcIter = blockChain->begin();

    if(filter == "Receptor" || filter == "Emisor")
    {
        if(type == "Igual")
        {
            if(compactTrie == nullptr)
            {
                while(bcIter != blockChain->end())
                {
                    Entry** entries = (*bcIter)->getEntries();
                    for(int i = 0; i < (*bcIter)->getSize(); ++i)
                    {
                        cout << entries[i]->print() << endl;

                        if(filter == "Emisor" && dynamic_cast<TransactionEntry*>(entries[i])->emisor == value1)
                        {
                            result.push_back(entries[i]);
                        }
                        else if(filter == "Receptor" && dynamic_cast<TransactionEntry*>(entries[i])->receptor == value1)
                        {
                            result.push_back(entries[i]);
                        }
                    }
                    ++bcIter;
                }
            } else {
                cout << "using compact Trie" << endl;
            }
        }
        else if (type == "Contiene")
        {
            cout << "Contain..." << endl;
            while(bcIter != blockChain->end())
            {
                Entry** entries = (*bcIter)->getEntries();
                for(int i = 0; i < (*bcIter)->getSize(); ++i)
                {
                    cout << entries[i]->print() << endl;

                    if(filter == "Emisor" && stringMatching(dynamic_cast<TransactionEntry*>(entries[i])->emisor, value1))
                    {
                        result.push_back(entries[i]);
                    }
                    else if(filter == "Receptor" && stringMatching(dynamic_cast<TransactionEntry*>(entries[i])->receptor, value1))
                    {
                        result.push_back(entries[i]);
                    }
                }
                ++bcIter;
            }
        }
    }

    else if(filter == "Monto" || filter == "Fecha")
    {
        if(type == "Igual")
        {
            while(bcIter != blockChain->end())
            {
                Entry** entries = (*bcIter)->getEntries();
                for(int i = 0; i < (*bcIter)->getSize(); ++i)
                {
                    if(filter == "Monto" && dynamic_cast<TransactionEntry*>(entries[i])->monto == stod(value1))
                    {
                        result.push_back(entries[i]);
                    }
                    else if(filter == "Fecha" && dynamic_cast<TransactionEntry*>(entries[i])->timestamp == datetime1)
                    {
                        result.push_back(entries[i]);
                    }
                }
                ++bcIter;
            }
        }
        else if(type == "Maximo")
        {
            size_t cmp = std::numeric_limits<size_t>::min();
            Entry* tmpEntry;
            while(bcIter != blockChain->end())
            {
                Entry** entries = (*bcIter)->getEntries();

                for(int i = 0; i < (*bcIter)->getSize(); ++i)
                {
                    if(filter == "Monto" && dynamic_cast<TransactionEntry*>(entries[i])->monto > cmp)
                    {
                        cmp = dynamic_cast<TransactionEntry*>(entries[i])->monto;
                        tmpEntry = entries[i];
                    }
                    else if(filter == "Fecha" && dynamic_cast<TransactionEntry*>(entries[i])->timestamp >= datetime1)
                    {
                        cmp = dynamic_cast<TransactionEntry*>(entries[i])->monto;
                        tmpEntry = entries[i];
                    }
                }
                ++bcIter;
            }
            result.push_back(tmpEntry);
        }
        else if(type == "Minimo")
        {
            size_t cmp = std::numeric_limits<size_t>::max();
            Entry* tmpEntry;
            while(bcIter != blockChain->end())
            {
                Entry** entries = (*bcIter)->getEntries();

                for(int i = 0; i < (*bcIter)->getSize(); ++i)
                {
                    if(filter == "Monto" && dynamic_cast<TransactionEntry*>(entries[i])->monto < cmp)
                    {
                        cmp = dynamic_cast<TransactionEntry*>(entries[i])->monto;
                        tmpEntry = entries[i];
                    }
                    else if(filter == "Fecha" && dynamic_cast<TransactionEntry*>(entries[i])->timestamp < datetime1)
                    {
                        cmp = dynamic_cast<TransactionEntry*>(entries[i])->monto;
                        tmpEntry = entries[i];
                    }
                }
                ++bcIter;
            }
            result.push_back(tmpEntry);
        }
        else if(type == "Entre")
        {
            while(bcIter != blockChain->end())
            {
                Entry** entries = (*bcIter)->getEntries();

                for(int i = 0; i < (*bcIter)->getSize(); ++i)
                {
                    if(filter == "Monto" && dynamic_cast<TransactionEntry*>(entries[i])->monto >= stod(value1) && dynamic_cast<TransactionEntry*>(entries[i])->monto <= stod(value2))
                    {
                        result.push_back(entries[i]);
                    }
                    else if(filter == "Fecha" && dynamic_cast<TransactionEntry*>(entries[i])->timestamp >= datetime1 && dynamic_cast<TransactionEntry*>(entries[i])->monto <= datetime2)
                    {
                        result.push_back(entries[i]);
                    }
                }
                ++bcIter;
            }

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

void MainWindow::redrawBlockChainOnUpdate(int blockId, int entryId, Block<BLOCK_SIZE> *updatedBlock)
{
    auto *mainView = ui->horizontalBlockDiv;
    this->clearBlockView();
    int i = 1;
    this->blockChainIterator = blockChain->begin();
    while (this->blockChainIterator != blockChain->end()) {
        auto *block = *blockChainIterator;
        auto *blockWidget = new BlockWidget(block, this);
        if (i == blockId) {
            // paint and update block
            cout << "repainting" << endl;
            blockWidget->changeStyle("background-color: rgba(103, 69, 69, 120)");
            blockWidget->repaint();
        }
        else if (i > blockId) {
            // repaint
            cout << "repainting" << endl;
            blockWidget->changeStyle("background-color: rgba(103, 69, 69, 120)");
            blockWidget->repaint();
        }
        else {
            blockWidget->changeStyle("background-color: rgba(69, 69, 103, 120)");
            blockWidget->repaint();
        }
        mainView->addWidget(blockWidget);
        this->lastBlockInserted = blockWidget;
        ++(this->blockChainIterator);
        ++i;
    }
    --blockChainIterator;

}

void MainWindow::clearBlockView()
{
    auto *mainView = ui->horizontalBlockDiv;
    for (int i = 0; i < mainView->count(); ++i) {
        auto blockWidget = qobject_cast<BlockWidget*>(mainView->itemAt(i)->widget());
        if (blockWidget != nullptr) {
            blockWidget->hide();
            blockWidget->deleteLater();
        }
        else {
            cout << "Error" << endl;
        }
    }
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

void MainWindow::onUpdateEntryButtonClick()
{
    auto *dialog = new QDialog();
    dialog->setModal(true);
    dialog->setGeometry(0, 0, 400, 400);
    dialog->setStyleSheet(dialogStyle);
    auto *findEntryForm = new FindEntryForm(dialog);
    connect(findEntryForm, &FindEntryForm::foundEntry, this, [this, dialog](int blockId, int entryId) {
        dialog->accept();
        this->updateEntryAtPosition(blockId, entryId);
    });
    findEntryForm->show();
    dialog->exec();
}

void MainWindow::updateEntryAtPosition(int blockId, int entryId)
{
    auto *dialog = new QDialog();
    dialog->setModal(true);
    dialog->setGeometry(0, 0, 600, 500);
    dialog->setStyleSheet(dialogStyle);
    auto *updateEntryForm = new UpdateEntryForm(dialog);
    connect(updateEntryForm, &UpdateEntryForm::updatedEntryValue, this, [blockId, entryId, this, dialog](Entry *newEntry){
        // update blockchain
        dialog->accept();
        auto *updatedBlock = this->blockChain->hackEntry(blockId, entryId, newEntry);
        this->redrawBlockChainOnUpdate(blockId, entryId, updatedBlock);
        cout << *(this->blockChain) << endl;
    });
    updateEntryForm->show();
    dialog->exec();
}

void MainWindow::validateBlockChain()
{
    QFuture<void> result = QtConcurrent::run([this](){
        auto mine = [&](){
            this->blockChain->validate();
        };
        TimedResult r = time_function(mine);
        this->updateTime(r);
    });
    // update view  when  the block chain has been  mined
    futureWatcher.setFuture(result);
}

void MainWindow::redrawBlockChainAfterMine()
{
    cout << "Redrawing everything" << endl;
    this->clearBlockView();
    //  redraw all
    auto *mainView = ui->horizontalBlockDiv;
    this->blockChainIterator = blockChain->begin();
    while (this->blockChainIterator != blockChain->end()) {
        auto *block = *blockChainIterator;
        auto *blockWidget = new BlockWidget(block, this);
        blockWidget->changeStyle("background-color: rgba(69, 69, 103, 120)");
        mainView->addWidget(blockWidget);
        this->lastBlockInserted = blockWidget;
        ++(this->blockChainIterator);
    }
    --blockChainIterator;
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
            ++blockChainIterator;
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

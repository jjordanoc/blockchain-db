#include "mainwindow.h"
#include "blockwidget.h"
#include <QCheckBox>
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
#include <QMovie>
#include <functional>



MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::MainWindow), blockChain(new BlockChain<BLOCK_SIZE>), blockChainIterator(blockChain->begin())
{
    setWindowIcon(QIcon("C:/Users/rojot/OneDrive/Escritorio/algoritmos_y_estructuras_de_datos/repo/proyecto-sha256/favicon.ico"));
    ui->setupUi(this);
    connect(ui->createEntryButton, &QPushButton::clicked, this, &MainWindow::onCreateBlockButtonClick);
    connect(ui->createIndexButton, &QPushButton::clicked, this, &MainWindow::onCreateIndexButtonClick);
    connect(ui->queryButton, &QPushButton::clicked, this, &MainWindow::onCreateQueryButtonClick);
    connect(ui->uploadFileButton, SIGNAL(clicked()), this, SLOT(uploadDataFromFile()));
    connect(ui->modifyEntryButton, SIGNAL(clicked()), this, SLOT(onUpdateEntryButtonClick()));
    connect(ui->mineButton, SIGNAL(clicked()), this, SLOT(validateBlockChain()));
    connect(&this->futureWatcher, &QFutureWatcher<void>::finished, this, &MainWindow::redrawBlockChainAfterMine);
    connect(&this->futureWatcher, &QFutureWatcher<void>::started, this, &MainWindow::showWaitingIcon);
    connect(ui->deleteEntryButton, SIGNAL(clicked()), this, SLOT(onDeleteEntryButtonClick()));
    ui->scrollArea->setWidget(ui->blockScrollAreaWidget);
    QMovie *movie = new QMovie("C:/Users/rojot/OneDrive/Escritorio/algoritmos_y_estructuras_de_datos/repo/proyecto-sha256/mining.gif");
    ui->miningLabel->setMovie(movie);
    movie->start();
    movie->setScaledSize(QSize(100, 100));
    ui->miningLabel->hide();
    indexes = {{"Emisor", nullptr}, {"Receptor", nullptr}, {"Fecha", nullptr}, {"Monto", nullptr}};
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
    dialog->setGeometry(0, 0, 400, 300);
    dialog->setStyleSheet(dialogStyle);
    cout << "Creating Index Form..." << endl;
    auto *createIndexForm = new CreateIndexForm(dialog);
    connect(createIndexForm, SIGNAL(submittedForm(QString, QString)), this, SLOT(createIndexes(QString,QString)));
    createIndexForm->show();
    dialog->exec();
}

void MainWindow::onCreateQueryButtonClick()
{
    auto *dialog = new QDialog();
    dialog->setModal(true);
    dialog->setGeometry(0, 0, 500, 300);
    dialog->setStyleSheet(dialogStyle);
    cout << "Creating Query Form..." << endl;
    auto *createQueryForm = new QueryForm(dialog);
    connect(createQueryForm, SIGNAL(emitData(std::unordered_map<std::string, std::string>)), this, SLOT(applyFilter(std::unordered_map<std::string, std::string>)));
    createQueryForm->show();
    dialog->exec();
}

void MainWindow::applyFilter(std::unordered_map<std::string, std::string> um)
{
    cout << "applying filter..." << endl;

    auto filter = um["filter"];
    auto type = um["type"];
    auto value1 = um["value1"];
    auto value2 = um["value2"]; // 2do parametro del entre
    auto datetime1 = stoull(um["datetime1"]);
    auto datetime2 = stoull(um["datetime2"]); // 2do parametro del entre
    auto aggregation = stoi(um["aggregate"]);

    std::vector<Entry*> result;
    auto bcIter = blockChain->begin();

    auto fun = [&](){
        // ------------------- Atributos de texto ------------------------- //
        if(filter == "Receptor" || filter == "Emisor")
        {
            // Igual
            if(type == "Igual")
            {
                if(indexes[filter] == nullptr || (indexes[filter] != nullptr && indexes[filter]->type != "AVL" && indexes[filter]->type != "Hash" && indexes[filter]->type != "BTree" && indexes[filter]->type != "Trie"))
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
                }
                else if(indexes[filter]->type == "AVL")
                {
                    cout << "AVL for Emisor/Receptor Igual" << endl;
                    auto cmpMayor = [&](shared_ptr<IndexT<string>> k1, shared_ptr<IndexT<string>> k2)
                    {
                        return k1->key > k2->key;
                    };

                    auto in = make_shared<IndexT<string>>(value1);
                    shared_ptr<IndexT<string>> indext = ((AVLTree<shared_ptr<IndexT<string>>, decltype(cmpMayor)>*)(indexes[filter]))->search(in);

                    for(auto& e : *(indext->values))
                    {
                        result.push_back(e);
                    }
                }
                else if(indexes[filter]->type == "Hash")
                {
                    cout << "HASH for Emisor/Receptor Igual" << endl;
                    auto indext = ((ChainHash<string, IndexT<string>>*)(indexes[filter]))->get(value1);
                    for(auto& e : *(indext.values))
                    {
                        result.push_back(e);
                    }
                }
                else if(indexes[filter]->type == "Trie")
                {
                    cout << "TRIE for Emisor/Receptor Igual" << endl;

                    auto cmp = [](IndexT<string>& i) -> string& {
                            return i.key;
                    };

                    IndexT<string> request(value1);
                    cout << boolalpha << ((CompactTrie<IndexT<string>, decltype(cmp)>*)(indexes[filter]))->find(request) << endl;
                    IndexT<string> res = ((CompactTrie<IndexT<string>, decltype(cmp)>*)(indexes[filter]))->searchEqual(request);
                    cout << boolalpha << (*(res.values)).empty() << endl;
                    for(const auto& e : *(res.values))
                    {
                        cout << e << endl;
                        result.push_back(e);
                    }
                }
            }
            // Contiene
            else if (type == "Contiene")
            {
                if(1)
                {
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
                else
                {
                    cout << "Unimplemented structure " << indexes[filter]->type << endl;
                }
            }
            // Inicia con
            else if(type == "Inicia con")
            {
                if(indexes[filter] == nullptr || (indexes[filter] != nullptr && indexes[filter]->type != "Trie"))
                {
                    while(bcIter != blockChain->end())
                    {
                        Entry** entries = (*bcIter)->getEntries();
                        for(int i = 0; i < (*bcIter)->getSize(); ++i)
                        {
                            cout << entries[i]->print() << endl;

                            if(filter == "Emisor" && stringStart(dynamic_cast<TransactionEntry*>(entries[i])->emisor, value1))
                            {
                                result.push_back(entries[i]);
                            }
                            else if(filter == "Receptor" && stringStart(dynamic_cast<TransactionEntry*>(entries[i])->receptor, value1))
                            {
                                result.push_back(entries[i]);
                            }
                        }
                        ++bcIter;
                    }
                }
                else if(indexes[filter]->type == "Trie")
                {
                    cout << "TRIE for Emisor/Receptor Igual" << endl;

                    auto cmp = [](IndexT<string>& i) -> string& {
                            return i.key;
                    };

                    IndexT<string> request(value1);
                    cout << boolalpha << ((CompactTrie<IndexT<string>, decltype(cmp)>*)(indexes[filter]))->find(request) << endl;
                    auto res = ((CompactTrie<IndexT<string>, decltype(cmp)>*)(indexes[filter]))->searchStartWith(request);

                    for (auto& e : res) {
                        for(auto& k : *(e.values))
                        {
                            result.push_back(k);
                        }
                    }
                }
                else
                {
                    cout << "Unimplemented structure " << indexes[filter]->type << endl;
                }
            }
            // Maximo
            else if (type == "Maximo")
            {
                if(indexes[filter] == nullptr || (indexes[filter] != nullptr && indexes[filter]->type != "MaxHeap" && indexes[filter]->type != "AVL" && indexes[filter]->type != "BTree"))
                {
                    string cmp = "";
                    Entry* tmpEntry;
                    while(bcIter != blockChain->end())
                    {
                        Entry** entries = (*bcIter)->getEntries();

                        for(int i = 0; i < (*bcIter)->getSize(); ++i)
                        {
                            if(filter == "Emisor" && dynamic_cast<TransactionEntry*>(entries[i])->emisor > cmp)
                            {
                                cmp = dynamic_cast<TransactionEntry*>(entries[i])->emisor;
                                cout << entries[i]->print() << endl;
                                tmpEntry = entries[i];
                            }
                            else if(filter == "Receptor" && dynamic_cast<TransactionEntry*>(entries[i])->receptor > cmp)
                            {
                                cmp = dynamic_cast<TransactionEntry*>(entries[i])->receptor;
                                cout << entries[i]->print() << endl;
                                tmpEntry = entries[i];
                            }
                        }
                        ++bcIter;
                    }
                    if(tmpEntry != nullptr) result.push_back(tmpEntry);
                }
                else if(indexes[filter]->type == "MaxHeap")
                {
                    cout << "MaxHeap for Emisor/Receptor Maximo" << endl;
                    auto cmp = [](Entry *e1, Entry *e2){
                        return ((TransactionEntry *)(e1))->emisor > ((TransactionEntry *)(e2))->emisor;
                    };

                    result.push_back(((Heap<Entry *, decltype(cmp)>*)(indexes[filter]))->top());
                }
                else if (indexes[filter]->type == "AVL") {
                    cout << "AVL for Emisor/Receptor Maximo" << endl;
                    auto cmpMayor = [&](shared_ptr<IndexT<string>> k1, shared_ptr<IndexT<string>> k2)
                    {
                        return k1->key > k2->key;
                    };

                    shared_ptr<IndexT<string>> indext = ((AVLTree<shared_ptr<IndexT<string>>, decltype(cmpMayor)>*)(indexes[filter]))->maxValue();

                    for(auto& e : *(indext->values))
                    {
                        result.push_back(e);
                    }
                }
            }
            // Minimo
            else if (type == "Minimo")
            {
                if(indexes[filter] == nullptr || (indexes[filter] != nullptr && indexes[filter]->type != "MinHeap" && indexes[filter]->type != "AVL" && indexes[filter]->type != "BTree"))
                {
                    string cmp = string(16, 'z');
                    Entry* tmpEntry;
                    while(bcIter != blockChain->end())
                    {
                        Entry** entries = (*bcIter)->getEntries();

                        for(int i = 0; i < (*bcIter)->getSize(); ++i)
                        {
                            if(filter == "Emisor" && dynamic_cast<TransactionEntry*>(entries[i])->emisor < cmp)
                            {
                                cmp = dynamic_cast<TransactionEntry*>(entries[i])->emisor;
                                tmpEntry = entries[i];
                            }
                            else if(filter == "Receptor" && dynamic_cast<TransactionEntry*>(entries[i])->receptor < cmp)
                            {
                                cmp = dynamic_cast<TransactionEntry*>(entries[i])->receptor;
                                tmpEntry = entries[i];
                            }
                        }
                        ++bcIter;
                    }
                    if(tmpEntry != nullptr) result.push_back(tmpEntry);
                }
                else if(indexes[filter]->type == "MinHeap")
                {
                    cout << "MinHeap for Emisor/Receptor Minimo" << endl;
                    auto cmp = [](Entry *e1, Entry *e2){
                        return ((TransactionEntry *)(e1))->emisor < ((TransactionEntry *)(e2))->emisor;
                    };

                    result.push_back(((Heap<Entry *, decltype(cmp)>*)(indexes[filter]))->top());
                }
                else if (indexes[filter]->type == "AVL") {
                    cout << "AVL for Emisor/Receptor Minimo" << endl;
                    auto cmpMayor = [&](shared_ptr<IndexT<string>> k1, shared_ptr<IndexT<string>> k2)
                    {
                        return k1->key > k2->key;
                    };

                    shared_ptr<IndexT<string>> indext = ((AVLTree<shared_ptr<IndexT<string>>, decltype(cmpMayor)>*)(indexes[filter]))->minValue();

                    for(auto& e : *(indext->values))
                    {
                        result.push_back(e);
                    }
                }
                else
                {
                    cout << "Unimplemented structure " << indexes[filter]->type << endl;
                }
            }
        }
        // ------------------- Atributos numéricos ------------------------- //
        else if(filter == "Monto" || filter == "Fecha")
        {
            // Igual
            if(type == "Igual")
            {
                if(indexes[filter] == nullptr || (indexes[filter] != nullptr && indexes[filter]->type != "AVL" && indexes[filter]->type != "Hash" && indexes[filter]->type != "BTree" && indexes[filter]->type != "Trie"))
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
                else if(indexes[filter]->type == "AVL")
                    {
                        cout << "AVL for Monto/Fecha Igual" << endl;
                        auto cmpMayor = [&](shared_ptr<IndexT<double>> k1, shared_ptr<IndexT<double>> k2)
                        {
                            return k1->key > k2->key;
                        };

                        auto in = make_shared<IndexT<double>>(stod(value1));
                        shared_ptr<IndexT<double>> indext = ((AVLTree<shared_ptr<IndexT<double>>, decltype(cmpMayor)>*)(indexes[filter]))->search(in);

                        for(auto& e : *(indext->values))
                        {
                            result.push_back(e);
                        }
                    }
                else if(indexes[filter]->type == "Hash")
                {
                    if(filter == "Monto")
                    {
                        cout << "HASH for Emisor/Receptor Igual" << endl;
                        auto indext = ((ChainHash<double, IndexT<double>>*)(indexes[filter]))->get(stod(value1));
                        for(auto& e : *(indext.values))
                        {
                            result.push_back(e);
                        }
                    }
                    else if(filter == "Fecha")
                    {
                        cout << "HASH for Emisor/Receptor Igual" << endl;
                        auto indext = ((ChainHash<size_t, IndexT<size_t>>*)(indexes[filter]))->get(datetime1);
                        for(auto& e : *(indext.values))
                        {
                            result.push_back(e);
                        }
                    }
                }
                else {
                    cout << "Unimplemented structure " << indexes[filter]->type << endl;
                }
            }
            // Maximo
            else if(type == "Maximo")
            {
                if(indexes[filter] == nullptr || (indexes[filter] != nullptr && indexes[filter]->type != "AVL" && indexes[filter]->type != "MaxHeap" && indexes[filter]->type != "BTree"))
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
                            else if(filter == "Fecha" && dynamic_cast<TransactionEntry*>(entries[i])->timestamp > cmp)
                            {
                                cmp = dynamic_cast<TransactionEntry*>(entries[i])->timestamp;
                                tmpEntry = entries[i];
                            }
                        }
                        ++bcIter;
                    }
                    if(tmpEntry != nullptr) result.push_back(tmpEntry);
                }
                else if(indexes[filter]->type == "MaxHeap")
                {
                    cout << "MaxHeap for Monto/Fecha Maximo" << endl;
                    if (filter == "Monto") {
                        auto cmp = [](Entry *e1, Entry *e2){
                            return ((TransactionEntry *)(e1))->monto > ((TransactionEntry *)(e2))->monto;
                        };
                        result.push_back(((Heap<Entry *, decltype(cmp)>*)(indexes[filter]))->top());
                    }
                    else if (filter == "Fecha") {
                        auto cmp = [](Entry *e1, Entry *e2){
                            return ((TransactionEntry *)(e1))->timestamp > ((TransactionEntry *)(e2))->timestamp;
                        };
                        result.push_back(((Heap<Entry *, decltype(cmp)>*)(indexes[filter]))->top());
                    }
                }
                else if (indexes[filter]->type == "AVL") {
                    cout << "AVL for Monto/Fecha Maximo" << endl;
                    auto cmpMayor = [&](shared_ptr<IndexT<double>> k1, shared_ptr<IndexT<double>> k2)
                    {
                        return k1->key > k2->key;
                    };
                    shared_ptr<IndexT<double>> indext = ((AVLTree<shared_ptr<IndexT<double>>, decltype(cmpMayor)>*)(indexes[filter]))->maxValue();

                    for(auto& e : *(indext->values))
                    {
                        result.push_back(e);
                    }
                }
                else
                {
                    cout << "Unimplemented structure " << indexes[filter]->type << endl;
                }
            }
            // Minimo
            else if(type == "Minimo")
            {
                if(indexes[filter] == nullptr || (indexes[filter] != nullptr && indexes[filter]->type != "AVL" && indexes[filter]->type != "MinHeap" && indexes[filter]->type != "BTree"))
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
                            else if(filter == "Fecha" && dynamic_cast<TransactionEntry*>(entries[i])->timestamp < cmp)
                            {
                                cmp = dynamic_cast<TransactionEntry*>(entries[i])->timestamp;
                                tmpEntry = entries[i];
                            }
                        }
                        ++bcIter;
                    }
                    if(tmpEntry != nullptr) result.push_back(tmpEntry);
                }
                else if(indexes[filter]->type == "MinHeap")
                {
                    cout << "MinHeap for Monto/Fecha Minimo" << endl;
                    if (filter == "Monto") {
                        auto cmp = [](Entry *e1, Entry *e2){
                            return ((TransactionEntry *)(e1))->monto < ((TransactionEntry *)(e2))->monto;
                        };
                        result.push_back(((Heap<Entry *, decltype(cmp)>*)(indexes[filter]))->top());
                    }
                    else if (filter == "Fecha") {
                        auto cmp = [](Entry *e1, Entry *e2){
                            return ((TransactionEntry *)(e1))->timestamp < ((TransactionEntry *)(e2))->timestamp;
                        };
                        result.push_back(((Heap<Entry *, decltype(cmp)>*)(indexes[filter]))->top());
                    }
                }
                else if (indexes[filter]->type == "AVL") {
                    cout << "AVL for Monto/Fecha Minimo" << endl;
                    auto cmpMayor = [&](shared_ptr<IndexT<double>> k1, shared_ptr<IndexT<double>> k2)
                    {
                        return k1->key > k2->key;
                    };
                    shared_ptr<IndexT<double>> indext = ((AVLTree<shared_ptr<IndexT<double>>, decltype(cmpMayor)>*)(indexes[filter]))->minValue();

                    for(auto& e : *(indext->values))
                    {
                        result.push_back(e);
                    }
                }
                else
                {
                    cout << "Unimplemented structure " << indexes[filter]->type << endl;
                }
            }
            // Entre
            else if(type == "Entre")
            {
                if(indexes[filter] == nullptr || (indexes[filter] != nullptr && indexes[filter]->type != "AVL" && indexes[filter]->type != "BTree"))
                {
                    while(bcIter != blockChain->end())
                    {
                        Entry** entries = (*bcIter)->getEntries();

                        for(int i = 0; i < (*bcIter)->getSize(); ++i)
                        {
                            if(filter == "Monto" && dynamic_cast<TransactionEntry*>(entries[i])->monto >= stod(value1)
                                    && dynamic_cast<TransactionEntry*>(entries[i])->monto <= stod(value2))
                            {
                                result.push_back(entries[i]);
                            }
                            else if(filter == "Fecha" && dynamic_cast<TransactionEntry*>(entries[i])->timestamp >= datetime1
                                    && dynamic_cast<TransactionEntry*>(entries[i])->timestamp <= datetime2)
                            {
                                result.push_back(entries[i]);
                            }
                        }
                        ++bcIter;
                    }
                }
                else if(indexes[filter]->type == "AVL")
                    {
                        cout << "using ALV for Monto/Fecha Entre" << endl;
                        auto cmpMayor = [&](shared_ptr<IndexT<double>> k1, shared_ptr<IndexT<double>> k2)
                        {
                            return k1->key > k2->key;
                        };

                        auto a = make_shared<IndexT<double>>(stod(value1));
                        auto b = make_shared<IndexT<double>>(stod(value2));
                        vector<shared_ptr<IndexT<double>>> indext = ((AVLTree<shared_ptr<IndexT<double>>, decltype(cmpMayor)>*)(indexes[filter]))->range_search(a, b);
                        for (auto &v_e : indext) {
                            for(auto& e : *(v_e->values))
                            {
                                result.push_back(e);
                            }
                        }

                    }
                else
                {
                    cout << "Unimplemented structure " << indexes[filter]->type << endl;
                }
            }
        }
    };

    TimedResult r = time_function(fun);
    this->updateTime(r);


    cout << "Termino el while" << endl;

    cout << "Showing dialog..." << endl;
    auto *dialog = new QDialog();
    dialog->setModal(true);
    dialog->setGeometry(0, 0, 550, 400);
    if (aggregation && !result.empty()) {
        double total = 0;
        for(auto& e : result)
        {
            total += ((TransactionEntry*)(e))->monto;
        }
        auto firstResult = ((TransactionEntry*)(result[0]));
        string usuario;
        if (filter == "Emisor") {
            usuario = firstResult->emisor;
        }
        else {
            usuario = firstResult->receptor;
        }
        auto* aggregatedView = new QueryDisplayView(usuario, total, dialog);
        aggregatedView->show();
        dialog->exec();
        return;
    }
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
    if(blockId > blockChain->size() ||  blockChain->back()->getFillCount() < entryId){
        ui->label_2->setText(QString::fromStdString("Last action: No valid position in the Blockchain"));
        ui->label_2->setStyleSheet(QString::fromStdString("QLabel { background-color: rgb(50, 50, 75); color : red; }"));
        return;
    }
    ui->label_2->setText(QString::fromStdString("No error registered"));
    ui->label_2->setStyleSheet(QString::fromStdString("QLabel { background-color: rgb(50, 50, 75); color : black; }"));
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

void MainWindow::deleteEntryAtPosition(int blockId, int entryId)
{
    // delete entry from all indexes
    Entry *entry = this->blockChain->searchEntry(blockId, entryId);
    TransactionEntry *casted = (TransactionEntry*) entry;
    for (auto &[attribute, index] : indexes) {
        if (index == nullptr) continue;
        if (index->type == "AVL") {
            if (attribute == "Emisor" || attribute == "Receptor") {
                auto cmpMayor = [&](shared_ptr<IndexT<string>> k1, shared_ptr<IndexT<string>> k2){
                    return k1->key > k2->key;
                };
                auto *tree = ((AVLTree<shared_ptr<IndexT<string>>, decltype(cmpMayor)>*)(index));
                if (attribute == "Emisor") {
                    auto remKey = make_shared<IndexT<string>>(casted->emisor);
                    auto remNode = tree->search(remKey);
                    (*remNode).values->remove(entry);
                }
                else if (attribute == "Receptor") {
                    auto remKey = make_shared<IndexT<string>>(casted->receptor);
                    auto remNode = tree->search(remKey);
                    (*remNode).values->remove(entry);
                }
            }
            else {
                auto cmpMayor = [&](shared_ptr<IndexT<double>> k1, shared_ptr<IndexT<double>> k2){
                    return k1->key > k2->key;
                };
                auto *tree = ((AVLTree<shared_ptr<IndexT<double>>, decltype(cmpMayor)>*)(index));
                if (attribute == "Monto") {
                    auto remKey = make_shared<IndexT<double>>(casted->monto);
                    auto remNode = tree->search(remKey);
                    (*remNode).values->remove(entry);
                }
                else if (attribute == "Fecha") {
                    auto remKey = make_shared<IndexT<double>>(casted->timestamp);
                    auto remNode = tree->search(remKey);
                    (*remNode).values->remove(entry);
                }
            }
        }
        else if (index->type == "Hash") {
            if (attribute == "Emisor" || attribute == "Receptor") {
                auto *hash = ((ChainHash<string, IndexT<string>>*)(index));
                if (attribute == "Emisor") {
                    auto remNode = hash->get(casted->emisor);
                    remNode.values->remove(entry);
                }
                else if (attribute == "Receptor") {
                    auto remNode = hash->get(casted->receptor);
                    remNode.values->remove(entry);
                }
            }
            else {
                auto *hash = ((ChainHash<double, IndexT<double>>*)(index));
                if (attribute == "Monto") {
                    auto remNode = hash->get(casted->monto);
                    remNode.values->remove(entry);
                }
                else if (attribute == "Fecha") {
                    auto remNode = hash->get(casted->timestamp);
                    remNode.values->remove(entry);
                }
            }
        }
    }
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
    ui->miningLabel->hide();
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

void MainWindow::showWaitingIcon()
{
    ui->miningLabel->show();
}

void MainWindow::createIndexes(QString qStructure, QString qAttribute)
{
    string attribute =  qAttribute.toStdString();
    string structure =  qStructure.toStdString();

    if (indexes[attribute] != nullptr) {
        delete indexes[attribute];
    }

    if (qStructure == "MaxHeap") {
        cout << "Creating MaxHeap on " << attribute << endl;
        if (attribute == "Emisor") {
            auto cmp = [](Entry *e1, Entry *e2){
                return ((TransactionEntry *)(e1))->emisor > ((TransactionEntry *)(e2))->emisor;
            };
            indexes[attribute] = new Heap<Entry *, decltype(cmp)>(this->blockChain->size(), cmp);
            indexes[attribute]->type = structure;
            auto bcIter = blockChain->begin();

            while(bcIter != blockChain->end())
            {
                Entry** entries = (*bcIter)->getEntries();

                for(int i = 0; i < (*bcIter)->getSize(); ++i)
                {
                    ((Heap<Entry *, decltype(cmp)>*)(indexes[attribute]))->push(entries[i]);
                }
                ++bcIter;
            }
            return;
        }
        else if (attribute == "Receptor") {
            auto cmp = [](Entry *e1, Entry *e2){
                return ((TransactionEntry *)(e1))->receptor > ((TransactionEntry *)(e2))->receptor;
            };
            indexes[attribute] = new Heap<Entry *, decltype(cmp)>(this->blockChain->size(), cmp);
            indexes[attribute]->type = structure;
            auto bcIter = blockChain->begin();

            while(bcIter != blockChain->end())
            {
                Entry** entries = (*bcIter)->getEntries();

                for(int i = 0; i < (*bcIter)->getSize(); ++i)
                {
                    ((Heap<Entry *, decltype(cmp)>*)(indexes[attribute]))->push(entries[i]);
                }
                ++bcIter;
            }
            return;
        }
        else if (attribute == "Monto") {
            std::function<bool(Entry*,Entry*)> cmp = [](Entry *e1, Entry *e2){
                return ((TransactionEntry *)(e1))->monto > ((TransactionEntry *)(e2))->monto;
            };
            indexes[attribute] = new Heap<Entry *, decltype(cmp)>(this->blockChain->size(), cmp);
            indexes[attribute]->type = structure;
            auto bcIter = blockChain->begin();

            while(bcIter != blockChain->end())
            {
                Entry** entries = (*bcIter)->getEntries();

                for(int i = 0; i < (*bcIter)->getSize(); ++i)
                {
                    ((Heap<Entry *, decltype(cmp)>*)(indexes[attribute]))->push(entries[i]);
                }
                ++bcIter;
            }
            return;
        }
        else if (attribute == "Fecha") {
            auto cmp = [](Entry *e1, Entry *e2){
                return ((TransactionEntry *)(e1))->timestamp > ((TransactionEntry *)(e2))->timestamp;
            };
            indexes[attribute] = new Heap<Entry *, decltype(cmp)>(this->blockChain->size(), cmp);
            indexes[attribute]->type = structure;
            auto bcIter = blockChain->begin();

            while(bcIter != blockChain->end())
            {
                Entry** entries = (*bcIter)->getEntries();

                for(int i = 0; i < (*bcIter)->getSize(); ++i)
                {
                    ((Heap<Entry *, decltype(cmp)>*)(indexes[attribute]))->push(entries[i]);
                }
                ++bcIter;
            }
            return;
        }
    }
    else if (qStructure == "MinHeap") {
        cout << "Creating MinHeap on " << attribute << endl;
        if (attribute == "Emisor") {
            auto cmp = [](Entry *e1, Entry *e2){
                return ((TransactionEntry *)(e1))->emisor < ((TransactionEntry *)(e2))->emisor;
            };
            indexes[attribute] = new Heap<Entry *, decltype(cmp)>(this->blockChain->size(), cmp);
            indexes[attribute]->type = structure;
            auto bcIter = blockChain->begin();

            while(bcIter != blockChain->end())
            {
                Entry** entries = (*bcIter)->getEntries();

                for(int i = 0; i < (*bcIter)->getSize(); ++i)
                {
                    ((Heap<Entry *, decltype(cmp)>*)(indexes[attribute]))->push(entries[i]);
                }
                ++bcIter;
            }
            return;
        }
        else if (attribute == "Receptor") {
            auto cmp = [](Entry *e1, Entry *e2){
                return ((TransactionEntry *)(e1))->receptor < ((TransactionEntry *)(e2))->receptor;
            };
            indexes[attribute] = new Heap<Entry *, decltype(cmp)>(this->blockChain->size(), cmp);
            indexes[attribute]->type = structure;
            auto bcIter = blockChain->begin();

            while(bcIter != blockChain->end())
            {
                Entry** entries = (*bcIter)->getEntries();

                for(int i = 0; i < (*bcIter)->getSize(); ++i)
                {
                    ((Heap<Entry *, decltype(cmp)>*)(indexes[attribute]))->push(entries[i]);
                }
                ++bcIter;
            }
            return;
        }
        else if (attribute == "Monto") {
            auto cmp = [](Entry *e1, Entry *e2){
                return ((TransactionEntry *)(e1))->monto < ((TransactionEntry *)(e2))->monto;
            };
            indexes[attribute] = new Heap<Entry *, decltype(cmp)>(this->blockChain->size(), cmp);
            indexes[attribute]->type = structure;
            auto bcIter = blockChain->begin();

            while(bcIter != blockChain->end())
            {
                Entry** entries = (*bcIter)->getEntries();

                for(int i = 0; i < (*bcIter)->getSize(); ++i)
                {
                    ((Heap<Entry *, decltype(cmp)>*)(indexes[attribute]))->push(entries[i]);
                }
                ++bcIter;
            }
            return;
        }
        else if (attribute == "Fecha") {
            auto cmp = [](Entry *e1, Entry *e2){
                return ((TransactionEntry *)(e1))->timestamp < ((TransactionEntry *)(e2))->timestamp;
            };
            indexes[attribute] = new Heap<Entry *, decltype(cmp)>(this->blockChain->size(), cmp);
            indexes[attribute]->type = structure;
            auto bcIter = blockChain->begin();

            while(bcIter != blockChain->end())
            {
                Entry** entries = (*bcIter)->getEntries();

                for(int i = 0; i < (*bcIter)->getSize(); ++i)
                {
                    ((Heap<Entry *, decltype(cmp)>*)(indexes[attribute]))->push(entries[i]);
                }
                ++bcIter;
            }
            return;
        }
    }
    else if (qStructure == "AVL")
    {
        cout << "Creating AVL on " << attribute << endl;
        if (attribute == "Emisor")
        {
            auto cmpMayor = [&](shared_ptr<IndexT<string>> k1, shared_ptr<IndexT<string>> k2){
                return k1->key > k2->key;
            };
            indexes[attribute] = new AVLTree<shared_ptr<IndexT<string>>, decltype(cmpMayor)>(cmpMayor);
            indexes[attribute]->type = structure;

            cout << "ALV CREATED" << endl;

            unordered_map<string, shared_ptr<IndexT<string>>> um;

            auto bcIter = blockChain->begin();

            while(bcIter != blockChain->end())
            {
                Entry** entries = (*bcIter)->getEntries();

                for(int i = 0; i < (*bcIter)->getSize(); ++i)
                {
                    string emisor = ((TransactionEntry*)(entries[i]))->emisor;
                    if(um.find(emisor) == um.end())
                    {
                        um[emisor] = make_shared<IndexT<string>>(emisor, entries[i]);
                    }
                    else
                    {
                        um[emisor]->insert(entries[i]);
                    }
                }
                ++bcIter;
            }

            for(auto& e : um)
            {
                ((AVLTree<shared_ptr<IndexT<string>>, decltype(cmpMayor)>*)(indexes[attribute]))->insert(e.second);
            }
            cout << "FINO termino avl" << endl;;
            return;
        }
        else if (attribute == "Receptor")
        {
            auto cmpMayor = [&](shared_ptr<IndexT<string>> k1, shared_ptr<IndexT<string>> k2){
                return k1->key > k2->key;
            };
            indexes[attribute] = new AVLTree<shared_ptr<IndexT<string>>, decltype(cmpMayor)>(cmpMayor);
            indexes[attribute]->type = structure;

            cout << "ALV CREATED" << endl;

            unordered_map<string, shared_ptr<IndexT<string>>> um;

            auto bcIter = blockChain->begin();

            while(bcIter != blockChain->end())
            {
                Entry** entries = (*bcIter)->getEntries();

                for(int i = 0; i < (*bcIter)->getSize(); ++i)
                {
                    string receptor = ((TransactionEntry*)(entries[i]))->receptor;
                    if(um.find(receptor) == um.end())
                    {
                        um[receptor] = make_shared<IndexT<string>>(receptor, entries[i]);
                    }
                    else
                    {
                        um[receptor]->insert(entries[i]);
                    }
                }
                ++bcIter;
            }

            for(auto& e : um)
            {
                ((AVLTree<shared_ptr<IndexT<string>>, decltype(cmpMayor)>*)(indexes[attribute]))->insert(e.second);
            }
            cout << "FINO termino avl" << endl;;
            return;
        }
        else if (attribute == "Monto")
        {
            auto cmpMayor = [&](shared_ptr<IndexT<double>> k1, shared_ptr<IndexT<double>> k2){
                return k1->key > k2->key;
            };
            indexes[attribute] = new AVLTree<shared_ptr<IndexT<double>>, decltype(cmpMayor)>(cmpMayor);
            indexes[attribute]->type = structure;

            cout << "ALV CREATED" << endl;

            unordered_map<double, shared_ptr<IndexT<double>>> um;

            auto bcIter = blockChain->begin();

            while(bcIter != blockChain->end())
            {
                Entry** entries = (*bcIter)->getEntries();

                for(int i = 0; i < (*bcIter)->getSize(); ++i)
                {
                    double monto = ((TransactionEntry*)(entries[i]))->monto;
                    if(um.find(monto) == um.end())
                    {
                        um[monto] = make_shared<IndexT<double>>(monto, entries[i]);
                    }
                    else
                    {
                        um[monto]->insert(entries[i]);
                    }
                }
                ++bcIter;
            }

            for(auto& e : um)
            {
                ((AVLTree<shared_ptr<IndexT<double>>, decltype(cmpMayor)>*)(indexes[attribute]))->insert(e.second);
            }
            cout << "FINO termino avl" << endl;;
            return;
        }
        else if (attribute == "Fecha")
        {
            auto cmpMayor = [&](shared_ptr<IndexT<double>> k1, shared_ptr<IndexT<double>> k2){
                return k1->key > k2->key;
            };
            indexes[attribute] = new AVLTree<shared_ptr<IndexT<double>>, decltype(cmpMayor)>(cmpMayor);
            indexes[attribute]->type = structure;

            cout << "ALV CREATED" << endl;

            unordered_map<double, shared_ptr<IndexT<double>>> um;

            auto bcIter = blockChain->begin();

            while(bcIter != blockChain->end())
            {
                Entry** entries = (*bcIter)->getEntries();

                for(int i = 0; i < (*bcIter)->getSize(); ++i)
                {
                    double fecha = ((TransactionEntry*)(entries[i]))->timestamp;
                    if(um.find(fecha) == um.end())
                    {
                        um[fecha] = make_shared<IndexT<double>>(fecha, entries[i]);
                    }
                    else
                    {
                        um[fecha]->insert(entries[i]);
                    }
                }
                ++bcIter;
            }

            for(auto& e : um)
            {
                ((AVLTree<shared_ptr<IndexT<double>>, decltype(cmpMayor)>*)(indexes[attribute]))->insert(e.second);
            }
            cout << "FINO termino avl" << endl;;
            return;
        }
    }
    else if (qStructure == "Trie")
    {
        if (attribute == "Emisor")
        {
            auto cmp = [](IndexT<string>& i) -> string& {
                    return i.key;
            };

            indexes[attribute] = new CompactTrie<IndexT<string>, decltype(cmp)>(cmp);
            indexes[attribute]->type = structure;

            cout << "TRIE CREATED" << endl;

            unordered_map<string, IndexT<string>> um;

            auto bcIter = blockChain->begin();

            while(bcIter != blockChain->end())
            {
                Entry** entries = (*bcIter)->getEntries();

                for(int i = 0; i < (*bcIter)->getSize(); ++i)
                {
                    string str = ((TransactionEntry*)(entries[i]))->emisor;
                    if(um.find(str) == um.end())
                    {
                        um[str] = IndexT<string>(str, entries[i]);
                    }
                    else
                    {
                        um[str].insert(entries[i]);
                    }
                }
                ++bcIter;
            }

            for(auto& e : um)
            {
                cout << "1-" << e.second.values->empty() << endl;
                ((CompactTrie<IndexT<string>, decltype(cmp)>*)(indexes[attribute]))->insert(e.second);
            }
            cout << "TRIE FINISIMO TERMINO" << endl;

            indexes[attribute]->type = structure;
            return;
        }
        else if (attribute == "Receptor")
        {

        }
        else if (attribute == "Monto")
        {

        }
        else if (attribute == "Fecha")
        {

        }
    }
    else if (qStructure == "Hash")
    {
        if (attribute == "Emisor")
        {
            indexes[attribute] = new ChainHash<string, IndexT<string>>();
            indexes[attribute]->type = structure;

            cout << "HASH CREATED" << endl;

            auto bcIter = blockChain->begin();

            while(bcIter != blockChain->end())
            {
                Entry** entries = (*bcIter)->getEntries();

                for(int i = 0; i < (*bcIter)->getSize(); ++i)
                {
                    string str = ((TransactionEntry*)(entries[i]))->emisor;
                    if(!((ChainHash<string, IndexT<string>>*)(indexes[attribute]))->find(str))
                    {
                        (*((ChainHash<string, IndexT<string>>*)(indexes[attribute]))).insert(str, IndexT<string>(str, entries[i]));
                    }
                    else
                    {
                        (*((ChainHash<string, IndexT<string>>*)(indexes[attribute]))).getRef(str).insert(entries[i]);
                    }
                }
                ++bcIter;
            }

            cout << "HASH FINISIMO TERMINO" << endl;


            return;
        }
        else if (attribute == "Receptor")
        {
            indexes[attribute] = new ChainHash<string, IndexT<string>>();
            indexes[attribute]->type = structure;

            cout << "HASH CREATED" << endl;

            auto bcIter = blockChain->begin();

            while(bcIter != blockChain->end())
            {
                Entry** entries = (*bcIter)->getEntries();

                for(int i = 0; i < (*bcIter)->getSize(); ++i)
                {
                    string str = ((TransactionEntry*)(entries[i]))->receptor;
                    if(!((ChainHash<string, IndexT<string>>*)(indexes[attribute]))->find(str))
                    {
                        (*((ChainHash<string, IndexT<string>>*)(indexes[attribute]))).insert(str, IndexT<string>(str, entries[i]));
                    }
                    else
                    {
                        (*((ChainHash<string, IndexT<string>>*)(indexes[attribute]))).getRef(str).insert(entries[i]);
                    }
                }
                ++bcIter;
            }

            cout << "HASH FINISIMO TERMINO" << endl;

            return;
        }
        else if (attribute == "Monto")
        {
            indexes[attribute] = new ChainHash<double, IndexT<double>>();
            indexes[attribute]->type = structure;

            cout << "HASH CREATED" << endl;

            auto bcIter = blockChain->begin();

            while(bcIter != blockChain->end())
            {
                Entry** entries = (*bcIter)->getEntries();

                for(int i = 0; i < (*bcIter)->getSize(); ++i)
                {
                    double atr = ((TransactionEntry*)(entries[i]))->monto;
                    if(!((ChainHash<double, IndexT<double>>*)(indexes[attribute]))->find(atr))
                    {
                        (*((ChainHash<double, IndexT<double>>*)(indexes[attribute]))).insert(atr, IndexT<double>(atr, entries[i]));
                    }
                    else
                    {
                        (*((ChainHash<double, IndexT<double>>*)(indexes[attribute]))).getRef(atr).insert(entries[i]);
                    }
                }
                ++bcIter;
            }

            cout << "HASH FINISIMO TERMINO" << endl;

            return;
        }
        else if (attribute == "Fecha")
        {
            indexes[attribute] = new ChainHash<size_t, IndexT<size_t>>();
            indexes[attribute]->type = structure;

            cout << "HASH CREATED" << endl;

            auto bcIter = blockChain->begin();

            while(bcIter != blockChain->end())
            {
                Entry** entries = (*bcIter)->getEntries();

                for(int i = 0; i < (*bcIter)->getSize(); ++i)
                {
                    size_t atr = ((TransactionEntry*)(entries[i]))->timestamp;
                    if(!((ChainHash<size_t, IndexT<size_t>>*)(indexes[attribute]))->find(atr))
                    {
                        (*((ChainHash<size_t, IndexT<size_t>>*)(indexes[attribute]))).insert(atr, IndexT<size_t>(atr, entries[i]));
                    }
                    else
                    {
                        (*((ChainHash<size_t, IndexT<size_t>>*)(indexes[attribute]))).getRef(atr).insert(entries[i]);
                    }
                }
                ++bcIter;
            }

            cout << "HASH FINISIMO TERMINO" << endl;

            return;
        }
    }
}

void MainWindow::onDeleteEntryButtonClick()
{
    auto *dialog = new QDialog();
    dialog->setModal(true);
    dialog->setGeometry(0, 0, 400, 400);
    dialog->setStyleSheet(dialogStyle);
    auto *findEntryForm = new FindEntryForm(dialog);
    connect(findEntryForm, &FindEntryForm::foundEntry, this, [this, dialog](int blockId, int entryId) {
        dialog->accept();
        this->deleteEntryAtPosition(blockId, entryId);
    });
    findEntryForm->show();
    dialog->exec();
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
    QString timeText = QString::number(r.duration) + " μs";
    ui->timeLabel->setText(timeText);
}

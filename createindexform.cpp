#include "createindexform.h"
#include "ui_createindexform.h"
#include <unordered_map>

enum structure {Trie, Btree, AVL, Hash, Heap};

CreateIndexForm::CreateIndexForm(BlockChain<BLOCK_SIZE> *_bc, CompactTrie *_ct, QWidget *parent) :
    QWidget(parent),
    bc(_bc),
    ct(_ct),
    ui(new Ui::CreateIndexForm)
{
    ui->setupUi(this);
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(submitForm()));
}

void CreateIndexForm::submitForm()
{
    if(bc == nullptr) return;
    std::unordered_map<std::string, structure> um = {std::make_pair("Trie", Trie),std::make_pair("Btree", Btree), std::make_pair("AVL", AVL),std::make_pair("Hash", Hash),std::make_pair("Heap", Heap)};
    std::cout << "Submitted form" << std::endl;
    QString structure = ui->structure->currentText();
    QString attribute = ui->atribute->currentText();
    std::cout << "Struct: " << structure.toStdString() << endl;
    std::cout << "Attribute: " << attribute.toStdString() << endl;

    enum structure s = um[structure.toStdString()];

    if(s == Trie){
        ct = new CompactTrie();
        auto iter = bc->begin();
        while(iter != bc->end()){
            break;
        }
    }
}

CreateIndexForm::~CreateIndexForm()
{
    delete ui;
}

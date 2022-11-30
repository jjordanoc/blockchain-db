#ifndef CREATEINDEXFORM_H
#define CREATEINDEXFORM_H
#include "globals.h"
#include "blockchain.h"
#include "compacttrie.h"
#include <QWidget>

namespace Ui {
class CreateIndexForm;
}

class CreateIndexForm : public QWidget
{
    Q_OBJECT

public:
    explicit CreateIndexForm(BlockChain<BLOCK_SIZE> *_bc, CompactTrie *_ct, QWidget *parent = nullptr);
    ~CreateIndexForm();
    BlockChain<BLOCK_SIZE> *bc;
    CompactTrie *ct;
private slots:
    void submitForm();
private:
    Ui::CreateIndexForm *ui;
};

#endif // CREATEINDEXFORM_H

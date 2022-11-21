#ifndef CREATEBLOCKFORM_H
#define CREATEBLOCKFORM_H
#include "globals.h"
#include "blockchain.h"
#include <QWidget>


namespace Ui {
class CreateBlockForm;
}

class CreateBlockForm : public QWidget
{
    Q_OBJECT

public:
    explicit CreateBlockForm(BlockChain<BLOCK_SIZE> *_bc, QWidget *parent = nullptr);
    ~CreateBlockForm();
private:
    Ui::CreateBlockForm *ui;
    BlockChain<BLOCK_SIZE> *bc;
private slots:
    void submitForm();
signals:
    void updatedBlockChain(Block<BLOCK_SIZE> *);
};

#endif // CREATEBLOCKFORM_H

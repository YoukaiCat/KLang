#include "ASTGraphDialog.h"
#include "ui_ASTGraphDialog.h"

#include <QGraphicsView>

ASTGraphDialog::ASTGraphDialog(QPixmap & pmap, QWidget * parent) :
    QDialog(parent),
    pmap(pmap),
    ui(new Ui::ASTGraphDialog)
{
    ui->setupUi(this);
    ui->graphicsView->setScene(&scene);
    item.setPixmap(pmap);
    scene.addItem(&item);
}

ASTGraphDialog::~ASTGraphDialog()
{
    delete ui;
}

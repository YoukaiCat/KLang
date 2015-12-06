#ifndef ASTGRAPHDIALOG_H
#define ASTGRAPHDIALOG_H

#include <QDialog>
#include <QPixmap>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

namespace Ui {
class ASTGraphDialog;
}

class ASTGraphDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ASTGraphDialog(QPixmap & pmap, QWidget * parent = 0);
    ~ASTGraphDialog();

private:
    QGraphicsScene scene;
    QGraphicsPixmapItem item;
    QPixmap & pmap;
    Ui::ASTGraphDialog * ui;
};

#endif // ASTGRAPHDIALOG_H

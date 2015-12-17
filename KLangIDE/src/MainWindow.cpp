/**
 * Copyright (C) 2015 VLadislav Mileshkin
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#include <math.h>

#include <QFileDialog>
#include <QMessageBox>

#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "ASTGraphVisualizer.h"
#include "ASTGraphDialog.h"

MainWindow::MainWindow(QWidget * parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    editor = new QTextEdit(this);

    highlighter = new SyntaxHighlighter(editor->document());

    auto example =
R"doc(Начало
Анализ а;
Анализ б;
Синтез в, г1, д123
а = 2.0 + 6.0 / 3.0 * 2.0 + 2.0
б = - а - (- а - а) + (- (а))
в = не а или не б
г1 = 0.0 и в
Окончание
)doc";

    editor->setFontPointSize(16);
    editor->setText(example);

    console = new QTextEdit(this);
    console->setFontPointSize(16);
    console->setReadOnly(true);

    editorSplitter = new QSplitter(Qt::Vertical, this);
    editorSplitter->addWidget(editor);
    editorSplitter->addWidget(console);

    help = new QTextBrowser(this);
    help->setFontPointSize(16);
    help->setSource(QUrl("qrc:/doc/help"));

    mainSplitter = new QSplitter(Qt::Horizontal, this);
    mainSplitter->addWidget(editorSplitter);
    mainSplitter->addWidget(help);

    mainLayout = new QHBoxLayout();
    mainLayout->addWidget(mainSplitter);

    centralWidget()->setLayout(mainLayout);

    filename = "";

    cursor = new QTextCursor(editor->document());

    connect(editor->document(), SIGNAL(contentsChanged()),
            this, SLOT(documentWasModified()));
}

void MainWindow::maybeSave()
{
//    if (editor->document()->isModified()) {
//        QMessageBox::StandardButton ret;
//        ret = QMessageBox::warning(this, tr("Application"),
//                     tr("The document has been modified.\n"
//                        "Do you want to save your changes?"),
//                     QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
//        if (ret == QMessageBox::Save)
//            save();
//    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNew_triggered()
{
    editor->clear();
}

void MainWindow::on_actionOpen_triggered()
{
    auto filename = QFileDialog::getOpenFileName(this, "Open File",
        QDir::homePath(), "Text files(*.txt *.proxy *.proxylist)");
    if (!filename.isEmpty()) {
        // open file
    }
}

void MainWindow::on_actionSave_triggered()
{

}

void MainWindow::on_actionSaveAs_triggered()
{
    auto filename = QFileDialog::getSaveFileName(this, "Save File",
        QDir::homePath(), "Text files (TXT)(*.txt)");
    if(!filename.isEmpty()) {
        // save file
    }
}

void MainWindow::on_actionClose_triggered()
{
    editor->clear();
}

void MainWindow::on_actionQuit_triggered()
{

}

void MainWindow::run()
{
    lexer = new Lexer(editor->toPlainText());
    auto tokens = lexer->tokenize();
    parser = new Parser(tokens);
    auto ast = parser->parse();
    interpreter = new Interpreter(ast);

    qDebug(ast->inspect().toStdString().c_str());

    astGraph = QPixmap::fromImage(ASTGraphVisualizer::renderASTAsGraph(ast));

    printResult(interpreter->evaluate());
}

void MainWindow::on_actionRun_triggered()
{
    clearErrorHighlighting();

    try {
        run();
    } catch (Error e) {
        onError(e);
    }
}

void MainWindow::on_actionShowASTAsGraph_triggered()
{
    clearErrorHighlighting();

    try {
        run();
        ASTGraphDialog(astGraph, this).exec();
    } catch (Error e) {
        onError(e);
    }
}

void MainWindow::onError(Error e)
{
    console->setText(QString::number(e.getErrorCode()) + ": " + e.getMessage());

    QTextCharFormat fmt;
    fmt.setBackground(Qt::red);
    fmt.setFontPointSize(16);

    cursor->setPosition(e.getBegin(), QTextCursor::MoveAnchor);
    cursor->setPosition(e.getEnd(), QTextCursor::KeepAnchor);
    cursor->setCharFormat(fmt);

    cleared = false;
}

void MainWindow::printResult(const shared_ptr<QMap<QString, double>> map) const
{
    console->setText("");
    QMapIterator<QString, double> i(*map);
    while (i.hasNext()) {
        i.next();
        auto val = i.value();
        QString strVal;
        //Если число круглое, то #number не будет выводить точку и хотябы один ноль.
        //Поэтому проверяем остаток от деления, и если число круглое, выводим строго один ноль после запятой.
        //В ином случае вывод адаптивный: выводятся только значащие цифры после запятой.
        if (floor(val) == val) {
             strVal = QString::number(val, 'f', 1);
        } else {
             strVal = QString::number(val);
        }
        console->setText(console->toPlainText() + i.key() + " = " + strVal + "\n");
    }
}

void MainWindow::documentWasModified()
{
    clearErrorHighlighting();
}

void MainWindow::clearErrorHighlighting()
{
    if (!cleared) {
        //Без этого слот бы вызывался рекурсивно
        cleared = true;
        QTextCharFormat fmt;
        fmt.setFontPointSize(16);
        cursor->setCharFormat(fmt);
    }
}

void MainWindow::on_actionAboutProgram_triggered()
{
    QMessageBox::about(this, tr("О программе"),
        tr("Интерпретатор и интегрированная среда разработки"));
}

void MainWindow::on_actionAboutQt_triggered()
{
    qApp->aboutQt();
}

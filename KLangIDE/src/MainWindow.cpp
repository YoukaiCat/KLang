/**
 * Copyright (C) 2015 VLadislav Mileshkin
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "ASTGraphDialog.h"

#include <math.h>

MainWindow::MainWindow(QWidget * parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    editor = new QTextEdit(this);

    highlighter = new SyntaxHighlighter(editor->document());

    editor->setText("Начало\nАнализ а\nа = 5.0  + 5.0 * 2.0 + 5.0\nОкончание\n");

    console = new QTextEdit(this);
    console->setReadOnly(true);

    editorSplitter = new QSplitter(Qt::Vertical, this);
    editorSplitter->addWidget(editor);
    editorSplitter->addWidget(console);

    help = new QTextBrowser(this);
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
    if (editor->document()->isModified()) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Application"),
                     tr("The document has been modified.\n"
                        "Do you want to save your changes?"),
                     QMessageBox::Save | QMessageBox::Discard |             QMessageBox::Cancel);
//        if (ret == QMessageBox::Save)
//            save();
    }
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

    qDebug(ast.inspect().toStdString().c_str());

    astGraph = QPixmap::fromImage(ast.printAsGraph());

    printResult(interpreter->evaluate());

    delete lexer;
    delete parser;
    delete interpreter;
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
    console->setText(e.getMessage());

    QTextCharFormat fmt;
    fmt.setBackground(Qt::red);

    cursor->setPosition(e.getBegin(), QTextCursor::MoveAnchor);
    cursor->setPosition(e.getEnd(), QTextCursor::KeepAnchor);
    cursor->setCharFormat(fmt);

    cleared = false;
}

void MainWindow::printResult(QMap<QString, double> map)
{
    console->setText("");
    QMapIterator<QString, double> i(map);
    while (i.hasNext()) {
        i.next();
        //Если число круглое, то #number не будет выводить точку и хотябы один ноль.
        //Поэтому проверяем остаток от деления, и если число круглое, выводим строго один ноль после запятой.
        //В ином случае вывод адаптивный: выводятся только значащие цифры после запятой.
        if (fmod(i.value(), 10) == 0) {
            console->setText(console->toPlainText() + i.key() + " = " + QString::number(i.value(), 'f', 1) + "\n");
        } else {
            console->setText(console->toPlainText() + i.key() + " = " + QString::number(i.value()) + "\n");
        }
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

/**
 * Copyright (C) 2015 VLadislav Mileshkin
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#include <math.h>

#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>

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

    mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(mainSplitter);

    centralWidget()->setLayout(mainLayout);

    cursor = new QTextCursor(editor->document());

    connect(editor->document(), SIGNAL(contentsChanged()),
            this, SLOT(documentWasModified()));

    currentFile = nullptr;
}

MainWindow::~MainWindow()
{
    delete cursor;
    delete ui;
}

void MainWindow::on_actionNew_triggered()
{
    clearEditor();
}

void MainWindow::on_actionOpen_triggered()
{
    auto filename = QFileDialog::getOpenFileName(this, "Open File",
        QDir::homePath(), "Text files (*.txt *.klang)");

    if (!filename.isEmpty()) {
        std::unique_ptr<QFile> file(new QFile(filename));
        currentFile = std::move(file);
        if (currentFile->open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(currentFile.get());
            QString text;
            text = in.readAll();
            currentFile->close();
            editor->setText(text);
            pathChanged(filename);
        }
    }
}

void MainWindow::on_actionSave_triggered()
{
    if (currentFile != nullptr) {
        if (currentFile->open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            QTextStream out(currentFile.get());
            out << editor->toPlainText();
            currentFile->close();
            setWindowModified(false);
        }
    } else {
        on_actionSaveAs_triggered();
    }
}

void MainWindow::on_actionSaveAs_triggered()
{
    auto filename = QFileDialog::getSaveFileName(this, "Save File",
        QDir::homePath() + "/new.klang", "Text files (*.txt *.klang)");

    if(!filename.isEmpty()) {
        std::unique_ptr<QFile> file(new QFile(filename));
        currentFile = std::move(file);
        if (currentFile->open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(currentFile.get());
            out << editor->toPlainText();
            currentFile->close();
            pathChanged(filename);
        }
    }
}

void MainWindow::on_actionClose_triggered()
{
    clearEditor();
}

void MainWindow::run()
{
    Lexer lexer(editor->toPlainText());
    auto tokens = lexer.tokenize();
    Parser parser(tokens);
    auto ast = parser.parse();
    Interpreter interpreter(ast);

    qDebug(ast->inspect().toStdString().c_str());

    astGraph = QPixmap::fromImage(ASTGraphVisualizer::renderASTAsGraph(ast));

    printResult(interpreter.evaluate());
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

void MainWindow::clearEditor()
{
    editor->clear();
    currentFile = nullptr;
}

void MainWindow::pathChanged(const QString & path)
{
    setWindowModified(false);
    setWindowFilePath(path);
}

void MainWindow::printResult(const shared_ptr<QMap<QString, shared_ptr<Variable>>> map) const
{
    console->setText("");
    auto variables = map->values();
    qSort(variables.begin(), variables.end(), [](shared_ptr<Variable> left, shared_ptr<Variable> right){
        return left->getIndex() < right->getIndex();
    });
    for (auto variable : variables) {
        if (variable->isInitialized()) {
            console->setText(console->toPlainText() + variable->getName() + " = " + variable->toString() + "\n");
        }
    }
}

void MainWindow::documentWasModified()
{
    setWindowModified(true);
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

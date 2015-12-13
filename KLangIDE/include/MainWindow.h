/**
 * Copyright (C) 2015 VLadislav Mileshkin
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>

#include <QSplitter>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QTextBrowser>
#include <QPixmap>

#include "Lexer.h"
#include "Parser.h"
#include "Interpreter.h"
#include "Error.h"
#include "SyntaxHighlighter.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget * parent = 0);
    ~MainWindow();
    void maybeSave();
    void save();

public slots:
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSaveAs_triggered();
    void on_actionClose_triggered();
    void on_actionQuit_triggered();

    void on_actionRun_triggered();
    void on_actionShowASTAsGraph_triggered();

    void on_actionAboutProgram_triggered();
    void on_actionAboutQt_triggered();

    void documentWasModified();

    void onError(Error e);

private:
    void run();
    void printResult(const shared_ptr<QMap<QString, double>> map) const;
    void clearErrorHighlighting();

    Ui::MainWindow * ui;

    QTextEdit * editor;
    QTextEdit * console;

    QSplitter * editorSplitter;

    QTextBrowser * help;

    QSplitter * mainSplitter;
    QHBoxLayout * mainLayout;

    QString filename;

    Lexer * lexer;
    Parser * parser;
    Interpreter * interpreter;

    SyntaxHighlighter * highlighter;

    QTextCursor * cursor;
    bool cleared = false;

    QPixmap astGraph;
};

#endif // MAINWINDOW_H

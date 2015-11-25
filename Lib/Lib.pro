QT += core gui widgets

TEMPLATE = lib
TARGET = klang

CONFIG += warn_on staticlib

include(../includes.pri)

SOURCES += \
    src/MainWindow.cpp \
    src/Token.cpp \
    src/Lexeme.cpp \
    src/Source.cpp \
    src/Lexer.cpp \
    src/Parser.cpp \
    src/Node.cpp \
    src/Interpreter.cpp \
    src/Error.cpp \
    src/SyntaxHighlighter.cpp \
    src/ASTGraphDialog.cpp

HEADERS += \
    include/MainWindow.h \
    include/Token.h \
    include/Lexeme.h \
    include/Source.h \
    include/Lexer.h \
    include/Parser.h \
    include/Node.h \
    include/Interpreter.h \
    include/Error.h \
    include/SyntaxHighlighter.h \
    include/ASTGraphDialog.h

FORMS += \
    ui/MainWindow.ui \
    ui/ASTGraphDialog.ui

include(../compiler.pri)

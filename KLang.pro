QT += core gui widgets

TARGET = KLang
TEMPLATE = app

CONFIG += warn_on debug

QMAKE_CXXFLAGS += -std=c++11 -pedantic -Wall -Wextra -O0 -ggdb -g3

SOURCES += main.cpp \
    MainWindow.cpp \
    Token.cpp \
    Lexeme.cpp \
    Source.cpp \
    Lexer.cpp \
    Parser.cpp \
    Node.cpp \
    Interpreter.cpp \
    Error.cpp \
    SyntaxHighlighter.cpp

HEADERS += \
    MainWindow.h \
    Token.h \
    Lexeme.h \
    Source.h \
    Lexer.h \
    Parser.h \
    Node.h \
    Interpreter.h \
    Error.h \
    SyntaxHighlighter.h

FORMS += \
    MainWindow.ui

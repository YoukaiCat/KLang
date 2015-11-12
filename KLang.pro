QT += core gui widgets

TARGET = KLang
TEMPLATE = app

CONFIG += warn_on

QMAKE_CXXFLAGS += -std=c++1y -pedantic -Wall -Wextra

CONFIG(debug, debug|release) {
    QMAKE_CXXFLAGS += -O0 -ggdb -g3
}

INCLUDEPATH += include/

SOURCES += \
    src/main.cpp \
    src/MainWindow.cpp \
    src/Token.cpp \
    src/Lexeme.cpp \
    src/Source.cpp \
    src/Lexer.cpp \
    src/Parser.cpp \
    src/Node.cpp \
    src/Interpreter.cpp \
    src/Error.cpp \
    src/SyntaxHighlighter.cpp

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
    include/SyntaxHighlighter.h

FORMS += \
    ui/MainWindow.ui

RESOURCES += KLang.qrc

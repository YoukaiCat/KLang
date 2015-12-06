QT += widgets

TEMPLATE = lib
TARGET = klang

CONFIG += warn_on shared

DEFINES += KLANG_LIBRARY

include(../QMake/includes.pri)

SOURCES += \
    src/Token.cpp \
    src/Lexeme.cpp \
    src/Source.cpp \
    src/Lexer.cpp \
    src/Parser.cpp \
    src/Node.cpp \
    src/Interpreter.cpp \
    src/Error.cpp

HEADERS += \
    include/Token.h \
    include/Lexeme.h \
    include/Source.h \
    include/Lexer.h \
    include/Parser.h \
    include/Node.h \
    include/Interpreter.h \
    include/Error.h \
    include/KLang_global.h

include(../QMake/compiler.pri)
include(../QMake/libs.pri)

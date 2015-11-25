QT += core gui widgets

TEMPLATE = app

SOURCES += main.cpp

LIBS += ../Lib/libklang.a

include(../includes.pri)
include(../compiler.pri)
include(../libs.pri)

RESOURCES += KLang.qrc

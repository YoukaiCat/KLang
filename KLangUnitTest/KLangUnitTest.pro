QT += testlib

CONFIG += testcase

TEMPLATE = app

SOURCES += KLangUnitTest.cpp

LIBS += ../Lib/libklang.a

include(../includes.pri)
include(../compiler.pri)
include(../libs.pri)

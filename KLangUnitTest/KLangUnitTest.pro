QT += testlib

CONFIG += testcase

TEMPLATE = app

SOURCES += KLangUnitTest.cpp

include(../QMake/compiler.pri)
include(../QMake/includes.pri)
include(../QMake/libklang.pri)
include(../QMake/libs.pri)
include(../QMake/installation.pri)

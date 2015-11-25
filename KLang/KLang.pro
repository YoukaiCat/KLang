QT += widgets

TEMPLATE = app

SOURCES += main.cpp

include(../QMake/compiler.pri)
include(../QMake/includes.pri)
include(../QMake/libklang.pri)
include(../QMake/libs.pri)
include(../QMake/installation.pri)

RESOURCES += KLang.qrc

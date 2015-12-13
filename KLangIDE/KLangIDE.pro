QT += widgets

TEMPLATE = app

INCLUDEPATH += include

SOURCES += \
    src/main.cpp \
    src/MainWindow.cpp \
    src/SyntaxHighlighter.cpp \
    src/ASTGraphDialog.cpp \
    src/ASTGraphVisualizer.cpp

HEADERS += \
    include/MainWindow.h \
    include/SyntaxHighlighter.h \
    include/ASTGraphDialog.h \
    include/ASTGraphVisualizer.h

FORMS += \
    ui/MainWindow.ui \
    ui/ASTGraphDialog.ui

include(../QMake/compiler.pri)
include(../QMake/includes.pri)
include(../QMake/libklang.pri)
include(../QMake/libs.pri)
include(../QMake/installation.pri)

RESOURCES += $$PWD/../KLang.qrc

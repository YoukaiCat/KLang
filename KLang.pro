QT += core gui widgets

TARGET = KLang
TEMPLATE = app

CONFIG += warn_on

QMAKE_CXXFLAGS += -DHAVE_STRING_H=1
!msvc:QMAKE_CXXFLAGS += -std=c++11 -pedantic -Wall -Wextra

CONFIG(debug, debug|release) {
    msvc:QMAKE_CXXFLAGS += /DEBUG
    !msvc:QMAKE_CXXFLAGS += -O0 -ggdb -g3
}

win32 {
    INCLUDEPATH += "C:/Program Files (x86)/Graphviz2.38/include/"
    GraphVizLibs.files = \
        "C:\Program Files (x86)\Graphviz2.38\bin\zlib1.dll" \
        "C:\Program Files (x86)\Graphviz2.38\bin\ltdl.dll" \
        "C:\Program Files (x86)\Graphviz2.38\bin\libexpat.dll" \
        "C:\Program Files (x86)\Graphviz2.38\bin\config6"
    CONFIG(debug, debug|release) {
        DESTDIR = debug
        GraphVizLibs.path = $$DESTDIR
        LIBS += \
            "C:/Program Files (x86)/Graphviz2.38/lib/debug/lib/gvc.lib" \
            "C:/Program Files (x86)/Graphviz2.38/lib/debug/lib/cgraph.lib"
        GraphVizLibs.files += \
            "C:\Program Files (x86)\Graphviz2.38\lib\debug\dll\cgraph.dll" \
            "C:\Program Files (x86)\Graphviz2.38\lib\debug\dll\gvc.dll" \
            "C:\Program Files (x86)\Graphviz2.38\lib\debug\dll\cdt.dll" \
            "C:\Program Files (x86)\Graphviz2.38\lib\debug\dll\Pathplan.dll" \
            "C:\Program Files (x86)\Graphviz2.38\lib\debug\dll\gvplugin_core.dll" \
            "C:\Program Files (x86)\Graphviz2.38\lib\debug\dll\gvplugin_dot_layout.dll" \
            "C:\Program Files (x86)\Graphviz2.38\lib\debug\dll\gvplugin_gdiplus.dll" \
            "C:\Program Files (x86)\Graphviz2.38\lib\debug\dll\gvplugin_gd.dll" \
            "C:\Program Files (x86)\Graphviz2.38\lib\debug\dll\cgraph.dll"

    }
    CONFIG(release, release|debug) {
        DESTDIR = release
        GraphVizLibs.path = $$DESTDIR
        LIBS += \
            "C:/Program Files (x86)/Graphviz2.38/lib/release/lib/gvc.lib" \
            "C:/Program Files (x86)/Graphviz2.38/lib/release/lib/cgraph.lib"
        GraphVizLibs.files += \
            "C:\Program Files (x86)\Graphviz2.38\lib\release\dll\cgraph.dll" \
            "C:\Program Files (x86)\Graphviz2.38\lib\release\dll\gvc.dll" \
            "C:\Program Files (x86)\Graphviz2.38\lib\release\dll\cdt.dll" \
            "C:\Program Files (x86)\Graphviz2.38\lib\release\dll\Pathplan.dll" \
            "C:\Program Files (x86)\Graphviz2.38\lib\release\dll\gvplugin_core.dll" \
            "C:\Program Files (x86)\Graphviz2.38\lib\release\dll\gvplugin_dot_layout.dll" \
            "C:\Program Files (x86)\Graphviz2.38\lib\release\dll\gvplugin_gdiplus.dll" \
            "C:\Program Files (x86)\Graphviz2.38\lib\release\dll\gvplugin_gd.dll" \
            "C:\Program Files (x86)\Graphviz2.38\lib\release\dll\cgraph.dll"
    }
    GraphVizLibs.CONFIG = no_check_exist
    INSTALLS += GraphVizLibs
} else {
    LIBS += -lgvc -lcgraph
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

RESOURCES += KLang.qrc

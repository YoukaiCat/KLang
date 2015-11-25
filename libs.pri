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
        DESTDIR = release\package
        TARGET.path = $$DESTDIR
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
    INSTALLS += GraphVizLibs
} else {
    LIBS += -lgvc -lcgraph
}

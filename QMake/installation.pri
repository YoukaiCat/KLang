win32 {
    CONFIG(release, release|debug) {
        DESTDIR = release\package
        TARGET.path = $$DESTDIR
    }
}

win32 {
    GraphVizLibs.files = \
        "C:\Program Files (x86)\Graphviz2.38\bin\zlib1.dll" \
        "C:\Program Files (x86)\Graphviz2.38\bin\ltdl.dll" \
        "C:\Program Files (x86)\Graphviz2.38\bin\libexpat.dll" \
        "C:\Program Files (x86)\Graphviz2.38\bin\config6"
    CONFIG(debug, debug|release) {
        DESTDIR = debug
        GraphVizLibs.path = $$DESTDIR
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

        KLangLib.path = $$DESTDIR
        KLangLib.files = ../KLang/release/klang.dll

        INSTALLS += KLangLib
    }
    else:CONFIG(release, release|debug) {
        DESTDIR = release\package
        GraphVizLibs.path = $$DESTDIR
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

        KLangLib.path = $$DESTDIR
        KLangLib.files = ../KLang/release/klang.dll

        INSTALLS += KLangLib
    }
    INSTALLS += GraphVizLibs
}

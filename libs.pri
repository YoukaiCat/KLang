win32 {
    CONFIG(debug, debug|release) {
        LIBS += \
            "C:/Program Files (x86)/Graphviz2.38/lib/debug/lib/gvc.lib" \
            "C:/Program Files (x86)/Graphviz2.38/lib/debug/lib/cgraph.lib"

    }
    CONFIG(release, release|debug) {
        LIBS += \
            "C:/Program Files (x86)/Graphviz2.38/lib/release/lib/gvc.lib" \
            "C:/Program Files (x86)/Graphviz2.38/lib/release/lib/cgraph.lib"
    }
} else {
    LIBS += -lgvc -lcgraph
}

win32 {
    CONFIG(debug, debug|release) {
        LIBS += ../Lib/release/klang.lib
    }
    CONFIG(release, release|debug) {
        LIBS += ../Lib/release/klang.lib
    }
} else {
    LIBS += ../Lib/libklang.a
}

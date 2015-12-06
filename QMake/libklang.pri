win32 {
    CONFIG(debug, debug|release) {
        LIBS += -L../KLang/debug/ -lklang
    }
    CONFIG(release, release|debug) {
        LIBS += -L../KLang/release/ -lklang
    }
} else {
    LIBS += -L../KLang/ -lklang
}

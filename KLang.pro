TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS = \
    Lib \
    KLang \
    KLangUnitTest

KLang.depends = Lib
KLangUnitTest.depends = Lib

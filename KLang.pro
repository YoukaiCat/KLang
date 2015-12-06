TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS = \
    KLang \
    KLangIDE \
    KLangUnitTest

KLangIDE.depends = KLang
KLangUnitTest.depends = KLang

CONFIG += testcase
CONFIG += parallel_test
TARGET = tst_qtextscriptengine

QT += core-private gui-private testlib

HEADERS += 
SOURCES += tst_qtextscriptengine.cpp 
INCLUDEPATH += $$QT_SOURCE_TREE/src/3rdparty/harfbuzz/src
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0

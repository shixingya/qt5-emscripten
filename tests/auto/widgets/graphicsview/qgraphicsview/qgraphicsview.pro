CONFIG += testcase
testcase.timeout = 500 # this test is slow
TARGET = tst_qgraphicsview

QT += widgets widgets-private testlib
QT += core-private gui-private

SOURCES  += tst_qgraphicsview.cpp tst_qgraphicsview_2.cpp
DEFINES += QT_NO_CAST_TO_ASCII

mac:CONFIG+=insignificant_test # QTBUG-26580
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0

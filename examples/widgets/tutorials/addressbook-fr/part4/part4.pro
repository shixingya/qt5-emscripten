SOURCES = addressbook.cpp \
          main.cpp
HEADERS = addressbook.h

QMAKE_PROJECT_NAME = abfr_part4

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/tutorials/addressbook-fr/part4
INSTALLS += target
QT += widgets


simulator: warning(This example might not fully work on Simulator platform)

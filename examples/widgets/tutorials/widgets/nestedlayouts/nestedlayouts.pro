SOURCES = main.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/tutorials/widgets/nestedlayouts
INSTALLS += target
QT += widgets


simulator: warning(This example might not fully work on Simulator platform)

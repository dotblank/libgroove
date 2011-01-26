# QJson library
CONFIG += link_pkgconfig
PKGCONFIG += QJson

QT += testlib network
TEMPLATE = app
DEPENDPATH += . ../../include/
INCLUDEPATH += . ../../include/

LIBS += $$OUT_PWD/../../src/libgroove.so

# run after link
QMAKE_POST_LINK = LD_LIBRARY_PATH=$$OUT_PWD/../../src/ ./$${TARGET}

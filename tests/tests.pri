QT += testlib network
TEMPLATE = app
DEPENDPATH += . ../../include/
INCLUDEPATH += . ../../include/

LIBS += ../../../libgroove-build-desktop/src/libgroove.so

# run after link
QMAKE_POST_LINK = ./$${TARGET}

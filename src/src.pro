#-------------------------------------------------
#
# Project created by QtCreator 2010-07-27T23:20:48
#
#-------------------------------------------------

QT       += network

TARGET = groove
TEMPLATE = lib
INCLUDEPATH = ../include/
DEPENDPATH = ../include/

# QJson library
CONFIG += link_pkgconfig
PKGCONFIG += QJson

DEFINES += LIBGROOVE_LIBRARY

SOURCES += grooveclient.cpp \
    groovesong.cpp \
    groovesearchmodel.cpp \
    grooveclient_p.cpp \
    grooveplaylistmodel.cpp \
    groovesongsmodel.cpp \
    grooverequest.cpp

HEADERS += grooveclient.h\
        libgroove_global.h \
    groovesong.h \
    groovesearchmodel.h \
    grooveclient_p.h \
    grooveplaylistmodel.h \
    groovesongsmodel.h \
    grooverequest.h

headers.files = $$HEADERS
headers.path = /usr/include
INSTALLS += headers

target.path = /usr/lib
INSTALLS += target

unix {
    CONFIG += create_pc create_prl
    pkgconfig.files = $${OUT_PWD}/$${TARGET}.pc
    pkgconfig.path = /usr/lib/pkgconfig
    INSTALLS += pkgconfig
}

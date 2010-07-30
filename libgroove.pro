#-------------------------------------------------
#
# Project created by QtCreator 2010-07-27T23:20:48
#
#-------------------------------------------------

#QT       += gui
QT       += network

TARGET = groove
TEMPLATE = lib

# QJson library
CONFIG += link_pkgconfig
PKGCONFIG += QJson

DEFINES += LIBGROOVE_LIBRARY

SOURCES += grooveclient.cpp \
    groovesong.cpp \
    groovesearchmodel.cpp \
    grooveclient_p.cpp

HEADERS += grooveclient.h\
        libgroove_global.h \
    groovesong.h \
    groovesearchmodel.h \
    grooveclient_p.h

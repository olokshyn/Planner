#-------------------------------------------------
#
# Project created by QtCreator 2017-07-27T17:06:47
#
#-------------------------------------------------

QT       -= core gui

TARGET = TreeSerialization
TEMPLATE = lib
CONFIG += staticlib

INCLUDEPATH += $$PWD/../DirectedRootedTree

SOURCES +=

HEADERS += TreeSerialization.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}

#-------------------------------------------------
#
# Project created by QtCreator 2017-07-18T15:25:29
#
#-------------------------------------------------

QT       -= core gui

TARGET = DirectedRootedTree
TEMPLATE = lib
CONFIG += staticlib

SOURCES +=

HEADERS += DirectedRootedTree.h \
    DirectedRootedTreeimpl.h \
    TreeNodeImpl.h \
    IteratorImpl.h \
    TreeAlgorithms.h \
    TreeAlgorithmsImpl.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}

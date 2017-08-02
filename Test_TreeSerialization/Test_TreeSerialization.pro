#-------------------------------------------------
#
# Project created by QtCreator 2017-07-28T16:47:35
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = TreeSerializationTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += $$PWD/../DirectedRootedTree
INCLUDEPATH += $$PWD/../TreeSerialization

SOURCES += TreeSerializationTest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

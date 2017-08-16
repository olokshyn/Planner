#-------------------------------------------------
#
# Project created by olokshyn on 2017-08-15T15:16:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SequencingModelTest
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


INCLUDEPATH += $$PWD/../DirectedRootedTree
INCLUDEPATH += $$PWD/../SequencingModel

SOURCES += \
        main.cpp

HEADERS +=

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../SequencingModel/release/ -lSequencingModel
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../SequencingModel/debug/ -lSequencingModel
else:unix: LIBS += -L$$OUT_PWD/../SequencingModel/ -lSequencingModel

INCLUDEPATH += $$PWD/../SequencingModel
DEPENDPATH += $$PWD/../SequencingModel

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../SequencingModel/release/libSequencingModel.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../SequencingModel/debug/libSequencingModel.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../SequencingModel/release/SequencingModel.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../SequencingModel/debug/SequencingModel.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../SequencingModel/libSequencingModel.a

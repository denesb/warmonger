include(../common.pri)
include(../deploy.pri)

TEMPLATE = app

TARGET = test_warmonger

QT += qml quick

LIBS += -L../log -L../core -llog -lcore

SOURCES += \
    Main.cpp

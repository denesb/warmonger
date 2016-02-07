include(../common.pri)

TEMPLATE = app

TARGET = test_warmonger

LIBS += -L../io -L../core -L../log -lio -lcore -llog

SOURCES += \
    Main.cpp

include(io/io.pri)

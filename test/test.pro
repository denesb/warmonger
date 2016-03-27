include(../common.pri)

TEMPLATE = app

TARGET = test_warmonger

LIBS += -L../io -L../core -L../log -lio -lcore -llog

HEADERS += \
    Util.h

SOURCES += \
    Util.cpp \
    Main.cpp

include(core/core.pri)
include(io/io.pri)

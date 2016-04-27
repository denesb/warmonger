include(../common.pri)

TEMPLATE = app

TARGET = test_warmonger

LIBS += -L../ui -L../io -L../core -L../log -lui -lio -lcore -llog

HEADERS += \
    Util.h

SOURCES += \
    Util.cpp \
    Main.cpp

include(core/core.pri)
include(io/io.pri)
include(ui/ui.pri)

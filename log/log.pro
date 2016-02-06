include(../common.pri)

TEMPLATE = lib

CONFIG += staticlib

HEADERS += \
    ConsoleHandler.h \
    Handler.h \
    Filter.h \
    Formatter.h \
    Logger.h \
    LogLevel.h \
    LogRecord.h \
    LogStream.h

SOURCES += \
    ConsoleHandler.cpp \
    Handler.cpp \
    Filter.cpp \
    Formatter.cpp \
    Logger.cpp \
    LogRecord.cpp \
    LogStream.cpp

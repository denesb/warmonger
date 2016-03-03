include(../common.pri)

TEMPLATE = lib

CONFIG += staticlib

HEADERS += \
    Context.h \
    Exception.h \
    File.h \
    Serializer.h \
    JsonSerializer.h \
    JsonUnserializer.h \
    Unserializer.h

SOURCES += \
    Context.cpp \
    File.cpp \
    JsonSerializer.cpp \
    JsonUnserializer.cpp

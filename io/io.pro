include(../common.pri)

TEMPLATE = lib

CONFIG += staticlib

HEADERS += \
    Context.h \
    Exception.h \
    Serializer.h \
    JsonSerializer.h \
    JsonUnserializer.h \
    Unserializer.h

SOURCES += \
    Exception.cpp \
    JsonSerializer.cpp \
    JsonUnserializer.cpp

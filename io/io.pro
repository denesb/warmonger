include(../common.pri)

TEMPLATE = lib

CONFIG += staticlib

HEADERS += \
    Serializer.h \
    Context.h \
    JsonSerializer.h \
    JsonUnserializer.h \
    QJsonUtil.h \
    Unserializer.h

SOURCES += \
    JsonSerializer.cpp \
    JsonUnserializer.cpp \
    QJsonUtil.cpp

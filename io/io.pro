include(../common.pri)

TEMPLATE = lib

CONFIG += staticlib

HEADERS += \
    Serializer.h \
    JsonSerializer.h \
    QJsonUtil.h

SOURCES += \
    JsonSerializer.cpp \
    QJsonUtil.cpp

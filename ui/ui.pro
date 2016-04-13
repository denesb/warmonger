include(../common.pri)
include(../deploy.pri)

TEMPLATE = app

TARGET = warmonger

QT += qml quick

LIBS += -L../log -L../core -llog -lcore

SOURCES += \
    Main.cpp \

HEADERS += \
    qtquick2applicationviewer.h \
    ApplicationContext.h \
    MiniMap.h \
    MapUtil.h

SOURCES += \
    qtquick2applicationviewer.cpp \
    ApplicationContext.cpp \
    MiniMap.cpp \
    MapUtil.cpp

OTHER_FILES += \
    qml/EditableMap.qml \
    qml/Game.qml \
    qml/GameItemInfo.qml \
    qml/GameSetup.qml \
    qml/InfoBar.qml \
    qml/ListEditGroup.qml \
    qml/Main.qml \
    qml/MapDetails.qml \
    qml/MapEditor.qml \
    qml/MapItemInfo.qml \
    qml/MapItemSelector.qml \
    qml/MapList.qml \
    qml/MapNodeEdit.qml \
    qml/NewGame.qml \
    qml/PointBar.qml \
    qml/SettlementEdit.qml \
    qml/TextEditGroup.qml \
    qml/UnitEdit.qml

qml.source = qml
qml.target =

worlds.source = ../worlds
worlds.target =

DEPLOYMENTFOLDERS += qml worlds

qtcAddDeployment()

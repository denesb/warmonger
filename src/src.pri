SRC = $$PWD

INCLUDEPATH += $$SRC

HEADERS += \
    $$SRC/Warmonger.h \
    $$SRC/core/JsonUtil.hpp \
    $$SRC/core/WorldItem.h \
    $$SRC/core/World.h \
    $$SRC/core/WorldLoader.h \
    $$SRC/core/TerrainType.h \
    $$SRC/core/SettlementType.h \
    $$SRC/core/UnitClass.h \
    $$SRC/core/DamageType.h \
    $$SRC/core/Weapon.h \
    $$SRC/core/Armor.h \
    $$SRC/core/UnitType.h \
    $$SRC/core/Faction.h \
    $$SRC/core/GameObject.h \
    $$SRC/core/MapTile.h \
    $$SRC/core/Unit.h \
    $$SRC/core/Settlement.h \
    $$SRC/core/Player.h \
    $$SRC/ui/model/WorldList.h

SOURCES += \
    $$SRC/main.cpp \
    $$SRC/Warmonger.cpp \
    $$SRC/core/JsonUtil.cpp \
    $$SRC/core/WorldItem.cpp \
    $$SRC/core/World.cpp \
    $$SRC/core/WorldLoader.cpp \
    $$SRC/core/TerrainType.cpp \
    $$SRC/core/SettlementType.cpp \
    $$SRC/core/UnitClass.cpp \
    $$SRC/core/DamageType.cpp \
    $$SRC/core/Weapon.cpp \
    $$SRC/core/Armor.cpp \
    $$SRC/core/UnitType.cpp \
    $$SRC/core/Faction.cpp \
    $$SRC/core/GameObject.cpp \
    $$SRC/core/MapTile.cpp \
    $$SRC/core/Unit.cpp \
    $$SRC/core/Settlement.cpp \
    $$SRC/core/Player.cpp \
    $$SRC/ui/model/WorldList.cpp

# Add more folders to ship with the application, here
qml.source = ui/qml
qml.target =

worlds.source = ../worlds
worlds.target = 

DEPLOYMENTFOLDERS = qml worlds

# Please do not modify the following two lines. Required for deployment.
include($$SRC/ui/qtquick2applicationviewer.pri)
qtcAddDeployment()

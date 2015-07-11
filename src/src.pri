SRC = $$PWD

INCLUDEPATH += $$SRC

HEADERS += \
    $$SRC/core/Armor.h \
    $$SRC/core/DamageType.h \
    $$SRC/core/Faction.h \
    $$SRC/core/GameObject.h \
    $$SRC/core/JsonUtil.hpp \
    $$SRC/core/Map.h \
    $$SRC/core/MapLoader.h \
    $$SRC/core/MapPosition.h \
    $$SRC/core/MapTile.h \
    $$SRC/core/Player.h \
    $$SRC/core/Settlement.h \
    $$SRC/core/SettlementType.h \
    $$SRC/core/TerrainType.h \
    $$SRC/core/Unit.h \
    $$SRC/core/UnitClass.h \
    $$SRC/core/UnitType.h \
    $$SRC/core/Util.h \
    $$SRC/core/Weapon.h \
    $$SRC/core/World.h \
    $$SRC/core/WorldItem.h \
    $$SRC/core/WorldLoader.h \
    $$SRC/Warmonger.h

SOURCES += \
    $$SRC/core/Armor.cpp \
    $$SRC/core/DamageType.cpp \
    $$SRC/core/Faction.cpp \
    $$SRC/core/GameObject.cpp \
    $$SRC/core/Map.cpp \
    $$SRC/core/MapLoader.cpp \
    $$SRC/core/MapPosition.cpp \
    $$SRC/core/MapTile.cpp \
    $$SRC/core/Player.cpp \
    $$SRC/core/Settlement.cpp \
    $$SRC/core/SettlementType.cpp \
    $$SRC/core/TerrainType.cpp \
    $$SRC/core/Unit.cpp \
    $$SRC/core/UnitClass.cpp \
    $$SRC/core/UnitType.cpp \
    $$SRC/core/Util.cpp \
    $$SRC/core/Weapon.cpp \
    $$SRC/core/World.cpp \
    $$SRC/core/WorldItem.cpp \
    $$SRC/core/WorldLoader.cpp \
    $$SRC/Main.cpp \
    $$SRC/Warmonger.cpp

OTHER_FILES += \
    src/ui/qml/GamePlay.qml \
    src/ui/qml/Main.qml \
    src/ui/qml/Map.qml \
    src/ui/qml/WorldList.qml

# Add more folders to ship with the application, here
qml.source = ui/qml
qml.target =

worlds.source = ../worlds
worlds.target = 

DEPLOYMENTFOLDERS = qml worlds

# Please do not modify the following two lines. Required for deployment.
include($$SRC/ui/qtquick2applicationviewer.pri)
qtcAddDeployment()

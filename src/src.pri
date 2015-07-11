INCLUDEPATH += src

HEADERS += \
    src/core/Armor.h \
    src/core/DamageType.h \
    src/core/Faction.h \
    src/core/GameObject.h \
    src/core/JsonUtil.hpp \
    src/core/Map.h \
    src/core/MapLoader.h \
    src/core/MapPosition.h \
    src/core/MapTile.h \
    src/core/Player.h \
    src/core/Settlement.h \
    src/core/SettlementType.h \
    src/core/TerrainType.h \
    src/core/Unit.h \
    src/core/UnitClass.h \
    src/core/UnitType.h \
    src/core/Util.h \
    src/core/Weapon.h \
    src/core/World.h \
    src/core/WorldItem.h \
    src/core/WorldLoader.h \
    src/Warmonger.h

SOURCES += \
    src/core/Armor.cpp \
    src/core/DamageType.cpp \
    src/core/Faction.cpp \
    src/core/GameObject.cpp \
    src/core/Map.cpp \
    src/core/MapLoader.cpp \
    src/core/MapPosition.cpp \
    src/core/MapTile.cpp \
    src/core/Player.cpp \
    src/core/Settlement.cpp \
    src/core/SettlementType.cpp \
    src/core/TerrainType.cpp \
    src/core/Unit.cpp \
    src/core/UnitClass.cpp \
    src/core/UnitType.cpp \
    src/core/Util.cpp \
    src/core/Weapon.cpp \
    src/core/World.cpp \
    src/core/WorldItem.cpp \
    src/core/WorldLoader.cpp \
    src/Main.cpp \
    src/Warmonger.cpp

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
include(ui/qtquick2applicationviewer.pri)
qtcAddDeployment()

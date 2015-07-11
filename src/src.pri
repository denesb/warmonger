# Don't add any other folders!
INCLUDEPATH += src

#
# All file list's are separated into groups by their module.
# Please keep the lists sorted!
#

# app
HEADERS += src/Warmonger.h

# core
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
    src/core/WorldLoader.h

# ui
HEADERS += \
    src/ui/qtquick2applicationviewer.h \
    src/ui/MapModel.h

# app
SOURCES += \
    src/Main.cpp \
    src/Warmonger.cpp

# core
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
    src/core/WorldLoader.cpp

# ui
SOURCES += \
    src/ui/qtquick2applicationviewer.cpp \
    src/ui/MapModel.cpp

# qml
OTHER_FILES += \
    src/ui/qml/GamePlay.qml \
    src/ui/qml/Main.qml \
    src/ui/qml/Map.qml \
    src/ui/qml/WorldList.qml \

# js
OTHER_FILES += \
    src/ui/js/Map.js

# deplyment
qml.source = src/ui/qml
qml.target =

js.source = src/ui/js
js.target = qml

worlds.source = worlds
worlds.target =

DEPLOYMENTFOLDERS = qml js worlds
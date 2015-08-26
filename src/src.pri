# Don't add any other folders!
INCLUDEPATH += src

#
# All file list's are separated into groups by their module.
# Please keep the lists sorted!
#

# app
HEADERS += src/Warmonger.h

# log
HEADERS += \
    src/log/ConsoleHandler.h \
    src/log/Handler.h \
    src/log/Filter.h \
    src/log/Formatter.h \
    src/log/Logger.h \
    src/log/LogLevel.h \
    src/log/LogRecord.h \
    src/log/LogStream.h

# core
HEADERS += \
    src/core/Armor.h \
    src/core/DamageType.h \
    src/core/Exception.h \
    src/core/Faction.h \
    src/core/GameEntity.h \
    src/core/GameObject.h \
    src/core/JsonUtil.hpp \
    src/core/Map.h \
    src/core/MapNode.h \
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
    src/core/WorldSurface.h

# ui
HEADERS += \
    src/ui/qtquick2applicationviewer.h \
    src/ui/UserInterface.h

# app
SOURCES += \
    src/Main.cpp \
    src/Warmonger.cpp

# log
SOURCES += \
    src/log/ConsoleHandler.cpp \
    src/log/Handler.cpp \
    src/log/Filter.cpp \
    src/log/Formatter.cpp \
    src/log/Logger.cpp \
    src/log/LogRecord.cpp \
    src/log/LogStream.cpp

# core
SOURCES += \
    src/core/Armor.cpp \
    src/core/DamageType.cpp \
    src/core/Exception.cpp \
    src/core/Faction.cpp \
    src/core/GameEntity.cpp \
    src/core/GameObject.cpp \
    src/core/JsonUtil.cpp \
    src/core/Map.cpp \
    src/core/MapNode.cpp \
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
    src/core/WorldSurface.cpp

# ui
SOURCES += \
    src/ui/qtquick2applicationviewer.cpp \
    src/ui/UserInterface.cpp

# qml
OTHER_FILES += \
    src/ui/qml/Main.qml \
    src/ui/qml/MapEditor.qml \
    src/ui/qml/MapItemSelector.qml

# js
OTHER_FILES += \
    src/ui/js/Map.js \
    src/ui/js/MapEditor.js \
    src/ui/js/MapItem.js \
    src/ui/js/Util.js

# deplyment
qml.source = src/ui/qml
qml.target =

js.source = src/ui/js
js.target = qml

worlds.source = worlds
worlds.target =

DEPLOYMENTFOLDERS = qml js worlds
